// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : TrainEngine.cpp
// Description : Contains the implementation of the 
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        :

#include "TrainEngine.h"
#include "SplineTrack.h"
#include "TrainControlls.h"
#include "TrainStopButton.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Static Variable Declarations
TStaticArray<UStaticMesh*, 4> ATrainEngine::StaticMeshRefs;

// Sets default values
ATrainEngine::ATrainEngine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	PlayerDetectionBoxes.Push(CreateDefaultSubobject<UBoxComponent>(TEXT("Player Detector")));
	PlayerDetectionBoxes[0]->SetupAttachment(RootComponent);
	PlayerDetectionBoxes[0]->InitBoxExtent(FVector(250.0f, 1500.0f, 350.0f));
	PlayerDetectionBoxes[0]->SetRelativeLocation(FVector(0.0f, 100.0f, 350.0f));
	// PlayerDetectionBoxes[0]->SetHiddenInGame(true);
	PlayerDetectionBoxes[0]->SetVisibility(false);
	PlayerDetectionBoxes[0]->OnComponentBeginOverlap.AddDynamic(this, &ATrainEngine::BeginEngineOverlap);
	PlayerDetectionBoxes[0]->OnComponentEndOverlap.AddDynamic(this, &ATrainEngine::EndEngineOverlap);

    for (int i = 0; i < 4; i++)
    {
	    if (!StaticMeshRefs[i])
	    {
	    	UStaticMesh* Mesh = nullptr;

	        if (i == 0)				// Passenger
	        {
	        	const ConstructorHelpers::FObjectFinder<UStaticMesh> FirstCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/Train_V2/Train_Car_Base_V2.Train_Car_Base_V2'"));
	        	Mesh = FirstCarMesh.Object;
	        }
	    	else if (i == 1)		// Flatbed
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> SecondCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/Train_V2/Train_Car_FlatBed_V2.Train_Car_FlatBed_V2'"));
	    		Mesh = SecondCarMesh.Object;
	        }
	    	else if (i == 2)		// Weapons
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> ThirdCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/Train_V2/Train_Car_Weapons_V2.Train_Car_Weapons_V2'"));
	    		Mesh = ThirdCarMesh.Object;
	        }
	    	else if (i == 3)		// Living Quarters
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> FourthCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/Train_V2/Train_Car_Windows_V2.Train_Car_Windows_V2'"));
	    		Mesh = FourthCarMesh.Object;
	        }

	    	StaticMeshRefs[i] = Mesh;
	    }
    }

	// Comment out once the BP functionality has been added
    if (!EngineMeshClass)
    {
	    static ConstructorHelpers::FClassFinder<AActor> File(TEXT("/Game/Production/Train/Art/BP_Train_Engine"));
	    if (File.Class)
	    {
		    EngineMeshClass = File.Class;
	    }
    }

	AbilitySystemComponent = CreateDefaultSubobject<UQRAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UQRAttributeSet>(TEXT("Attributes"));
}

#pragma region GAS
void ATrainEngine::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
	ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamagerCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamagerCauser);
}

void ATrainEngine::HandleHealthChanged(float Deltavalue, const FGameplayTagContainer& EventTags)
{
	OnHealthChanged(Deltavalue, EventTags);
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

	if (EngineMeshClass)
	{
		EngineMeshActor = GetWorld()->SpawnActor<AActor>(EngineMeshClass);
		EngineMeshActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	// Initialising variables
	TimeSinceStart = 0.0f;
	bHasStartedMoving = false;
	SplineLength = -5;

	// Checking for a spline reference and connecting if one is detected
	if (!TrackActorRef) {}
	else if (USplineComponent* TempSplineRef = Cast<USplineComponent>(TrackActorRef->GetRootComponent()); TempSplineRef != nullptr)
	{
		TrackSplineRef = TempSplineRef;
	}

	if (TrackSplineRef)
	{
		//GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, TEXT("Train Spline Ref populated"));
		SplineLength = TrackSplineRef->GetSplineLength();

		SetActorLocation(TrackSplineRef->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
		SetActorRotation(TrackSplineRef->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World) - FRotator(0.0f, 90.0f, 0.0f));
	}
	
	FTimerHandle TrainStartHandle;
	GetWorld()->GetTimerManager().SetTimer(TrainStartHandle, [&]()
	{
		if (TrackSplineRef)
		{
			bHasStartedMoving = true;
			bTrainMoving = true;
		}
	}, (StartDelayTime >= 1) ? StartDelayTime : 0.1f, false);
	
	// Spawning in the Carriages // Doesn't fire if CarriageCount <= 0
	for (int i = 0; i < CarriageCount; i++)
	{
		ATrainCarriage* TempRef = Cast<ATrainCarriage>(GetWorld()->SpawnActor(ATrainCarriage::StaticClass()));
		const int CarriageClassCount = CarriageMeshClasses.IsEmpty() ? 1 : CarriageMeshClasses.Num();
		TempRef->InitialiseFromEngine(i, DistanceFromFront + DistanceBetweenCarriages * i, CarriageMeshClasses[i % CarriageClassCount], TrackSplineRef, this);
		
		PlayerDetectionBoxes.Push(TempRef->GetPlayerDetectionComponent());

		CarriageRefs.Push(TempRef);
	}
	
	if (AbilitySystemComponent)
	{
		AddStartupGameplayAbilities();
	}

	EngineStopButton = Cast<ATrainStopButton>(GetWorld()->SpawnActor(ATrainStopButton::StaticClass()));
	EngineStopButton->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	EngineStopButton->SetActorRelativeLocation(FVector(0.0f, -600.0f, 420.0f));

	TrainControls = Cast<ATrainControlls>(GetWorld()->SpawnActor(ATrainControlls::StaticClass()));
	TrainControls->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	TrainControls->SetActorRelativeLocation(FVector(-150.0f, -740.0f, 240.0f));

	UpdateObjectiveText();

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass()));
	}

	TArray<AActor*> OverlappingActors;
	PlayerDetectionBoxes[0]->GetOverlappingActors(OverlappingActors);

	for (int i = 0; i < OverlappingActors.Num(); ++i)
	{
		if (OverlappingActors[i] == PlayerRef)
		{
			EnableTrainMovementTimer();
		}
	}
}

void ATrainEngine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(PlayerDetectionTimerHandle);

}

// Called every frame
void ATrainEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Standard Tick Operation
    if (bHasStartedMoving && bPlayerOnTrain && !bObjectiveLock)
    {
    	if (bTrainMoving)
    	{
    		TimeSinceStart += (DeltaTime * TrainSpeedModifier);
    	}
    	
	    const float TimerTrack = TimeSinceStart / TimeToComplete;

    	const float CurrentSplineProgress = FMath::Lerp(0, SplineLength, TimerTrack);

    	const FVector CurrentSplineVector = TrackSplineRef->GetLocationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World);
    	FRotator CurrentSplineRotator = TrackSplineRef->GetRotationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World) - FRotator(0.0f, 90.0f, 0.0f);
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
void ATrainEngine::ToggleTrainStop()
{
	if (!bObjectiveLock)
	{
		bTrainMoving = !bTrainMoving;
	}
}

/**
 * @brief
 * This function does nothing
 * @param NewSpeed 
 */
void ATrainEngine::SetTrainSpeed(ETrainSpeed NewSpeed)
{
	switch (NewSpeed)
	{
	case ETrainSpeed::Slow:
		TrainSpeedModifier = 0.5f;
		break;
	case ETrainSpeed::Standard:
		TrainSpeedModifier = 1.0f;
		break;
	case ETrainSpeed::Fast:
		TrainSpeedModifier = 2.0f;
		break;
	default:
		TrainSpeedModifier = 1.0f;
		break;
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

UBoxComponent* ATrainEngine::GetEngineDetectionComponent()
{
	return PlayerDetectionBoxes[0];
}

ATrainCarriage* ATrainEngine::GetLastCarriage()
{
	return CarriageRefs.IsEmpty() ? nullptr : CarriageRefs.Last();
}

void ATrainEngine::DisableMovement()
{
	bObjectiveLock = true;
}

void ATrainEngine::EnableMovement()
{
	bObjectiveLock = false;
}

void ATrainEngine::BeginEngineOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!bPlayerOnTrain && OtherActor == PlayerRef)
	{
		EnableTrainMovementTimer();

		// PlayerRef->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ATrainEngine::EndEngineOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (bPlayerOnTrain && OtherActor == PlayerRef)
	{
		DisableTrainMovementTimer();
	}
}

bool ATrainEngine::CheckTrainForPlayer()
{
	for (int i = 0; i < PlayerDetectionBoxes.Num(); i++)
	{
		TArray<AActor*> OverlappingActors;
		PlayerDetectionBoxes[i]->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());

		for (int j = 0; j < OverlappingActors.Num(); j++)
		{
			if (Cast<APawn>(OverlappingActors[j])->IsPlayerControlled())
			{
				return true;
			}
		}
	}
	
	return false;
}

void ATrainEngine::EnableTrainMovementTimer()
{
	GetWorldTimerManager().SetTimer(PlayerDetectionTimerHandle, [&]()
	{
		bPlayerOnTrain = true;
	}, 0.75f, false);
}

void ATrainEngine::DisableTrainMovementTimer()
{
	GetWorldTimerManager().SetTimer(PlayerDetectionTimerHandle, [&]()
	{
		if (!CheckTrainForPlayer())
		{
			bPlayerOnTrain = false;

			// PlayerRef->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
	}, 2.0f, true);
}
