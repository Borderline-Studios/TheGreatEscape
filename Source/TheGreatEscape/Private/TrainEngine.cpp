// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : TrainEngine.cpp
// Description : Contains the implementation of the 
// Author      : Borderline Studios - (person(s) working on file)
// Mail        :

#include "TrainEngine.h"

// Forward Declarations
#include "SplineTrack.h"

#include "TrainControlls.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrainEngine::ATrainEngine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectionComponent->InitBoxExtent(FVector(250.0f, 1500.0f, 350.0f));
	PlayerDetectionComponent->SetRelativeLocation(FVector(0.0f, 100.0f, 350.0f));
	PlayerDetectionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrainEngine::BeginCarOverlap);
	PlayerDetectionComponent->OnComponentEndOverlap.AddDynamic(this, &ATrainEngine::EndCarOverlap);

	AbilitySystemComponent = CreateDefaultSubobject<UQRAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UQRAttributeSet>(TEXT("Attributes"));
}

#pragma region GAS
void ATrainEngine::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
	ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
}

void ATrainEngine::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	OnHealthChanged(DeltaValue, EventTags);
}

void ATrainEngine::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);
	if(GetLocalRole() == ROLE_Authority && !bAbilitiesInitalized)
	{
		//Grant Abilities, but only on Server
		for(TSubclassOf<UQRGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			if(StartupAbility)
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
					StartupAbility, 1,
					static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}

	for (const TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);

		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get(), AbilitySystemComponent);
		}

		bAbilitiesInitalized = true;
	}
}

UAbilitySystemComponent* ATrainEngine::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
#pragma endregion

// Called when the game starts or when spawned
void ATrainEngine::BeginPlay()
{
	Super::BeginPlay();
	
	SetCurrentWorld(GetWorld());

	// Initialising variables
	TimeSinceStart = 0.0f;
	bStartedMoving = false;
	SplineLength = -5;

	// Checking for a spline reference and connecting if one is detected
	if (!WorldSplineRef) {}
	else if (USplineComponent* TempSplineRef = Cast<USplineComponent>(WorldSplineRef->GetSpline()); TempSplineRef != nullptr)
	{
		SplineRef = TempSplineRef;

		SplineLength = SplineRef->GetSplineLength();

		SetActorLocation(SplineRef->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
		SetActorRotation(SplineRef->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World) - FRotator(0.0f, 90.0f, 0.0f));

		bStartedMoving = true;
		 // if (true)
		 // {
			// 	bTrainMoving = true;
		 // }

		if (MovementCurve)
		{
			FOnTimelineFloat ProgressFunction;
			ProgressFunction.BindUFunction(this, TEXT("ProcessTrainSpeedTimeline"));
			TrainSpeedModificationTimeline.AddInterpFloat(MovementCurve, ProgressFunction);
		
			TrainSpeedModificationTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
		}
	}
	
	// Spawning in the Carriages // Doesn't fire if CarriageCount <= 0
	for (int i = 0; i < CarriageCount; i++)
	{
		ATrainCarriage* TempRef = Cast<ATrainCarriage>(GetWorld()->SpawnActor(TrainCarriageClass));
		TempRef->InitialiseFromEngine(i, DistanceFromFront + DistanceBetweenCarriages * i, StaticMeshRefs[i % StaticMeshRefs.Num()], SplineRef, this);
		
		CarriageRefs.Push(TempRef);
	}
	
	if (AbilitySystemComponent)
	{
		AddStartupGameplayAbilities();
	}
	
	TrainControls = Cast<ATrainControlls>(GetWorld()->SpawnActor(TrainControlsClass));
	TrainControls->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	TrainControls->SetActorRelativeLocation(FVector(-150.0f, -740.0f, 240.0f));

	UpdateObjectiveText();

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass()));
	}

	if (EngineMesh)
	{
		CarMesh->SetStaticMesh(EngineMesh);
	}
	
	if (!EngineRef)
	{
		EngineRef = this;
	}

	EnableTrainMovementTimer();
}

void ATrainEngine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATrainEngine::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void ATrainEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TrainSpeedModificationTimeline.IsPlaying())
	{
		TrainSpeedModificationTimeline.TickTimeline(DeltaTime);
	}

	// Standard Tick Operation
    if (bStartedMoving && GetPlayerOnTrain() && !bObjectiveLocked)
    {
    	if (!bTrainMoving) return;
    	
    	TimeSinceStart += (DeltaTime * TrainSpeedModifier);
    	
	    const float TimerTrack = TimeSinceStart / TimeToComplete;

    	const float CurrentSplineProgress = FMath::Lerp(0, SplineLength, TimerTrack);

    	const FVector CurrentSplineVector = SplineRef->GetLocationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World);
    	FRotator CurrentSplineRotator = SplineRef->GetRotationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World) - FRotator(0.0f, 90.0f, 0.0f);
    	CurrentSplineRotator.Pitch = 0;
    	
    	SetActorLocation(CurrentSplineVector);
    	SetActorRotation(CurrentSplineRotator);

        for (int i = 0; i < CarriageRefs.Num(); i++)
        {
	        CarriageRefs[i]->ProcessMovement(CurrentSplineProgress);
        }
    	
    	if (!UKismetMathLibrary::InRange_FloatFloat(TimerTrack, 0.0, 1.0, false, true))
    	{
    		TimeSinceStart -= TimeToComplete;
    	}
    }
}

/**
 * @brief This function does nothing
 */
void ATrainEngine::ToggleTrainStop(const bool bExtendLeft, const bool bExtendRight)
{
	if (!bObjectiveLocked)
	{
		MovePlatforms(bTrainMoving && bExtendLeft, bTrainMoving && bExtendRight);
		bTrainMoving = !bTrainMoving;
		
		if (TrainControls)
		{
			TrainControls->PlayLeverSound();
			TrainControls->UpdateHandleRotation(bTrainMoving);
		}
	}
}

/**
 * @brief 
 * @param NewText 
 */
void ATrainEngine::UpdateObjectiveText(FString NewText)
{
	NewText.Append(" ");
	CurrentObjectiveMessage = NewText;
}

UBoxComponent* ATrainEngine::GetEngineDetectionComponent() const
{
	return PlayerDetectionComponent;
}

ATrainCarriage* ATrainEngine::GetLastCarriage()
{
	return CarriageRefs.IsEmpty() ? nullptr : CarriageRefs.Last();
}

void ATrainEngine::DisableMovement()
{
	bObjectiveLocked = true;
}

void ATrainEngine::EnableMovement()
{
	bObjectiveLocked = false;
}

bool ATrainEngine::GetTrainMoving() const
{
	return bTrainMoving;
}

void ATrainEngine::ChangeTrainSpeedModifier(float NewModifier)
{
	CurrentModifier = TrainSpeedModifier;
	TargetModifier = NewModifier;

	TrainSpeedModificationTimeline.PlayFromStart();
}

void ATrainEngine::ResetTrainSpeed()
{
	CurrentModifier = TrainSpeedModifier;
	TargetModifier = DefaultTrainSpeedModifier;

	TrainSpeedModificationTimeline.PlayFromStart();
}

void ATrainEngine::BeginCarOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::BeginCarOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ATrainEngine::EndCarOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::EndCarOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void ATrainEngine::ProcessTrainSpeedTimeline(float TimelineProgress)
{
	TrainSpeedModifier = FMath::Lerp(CurrentModifier, TargetModifier, TimelineProgress);
}
