// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainEngine.h"
#include "SplineTrack.h"
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

	EngineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Carriage Mesh"));
	EngineMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Production/Train/Temporary-Meshes/Train/Engine/S_Train_Merged.S_Train_Merged'"));
	EngineMesh->SetStaticMesh(MeshObj.Object);

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FUCK BOX"));
	Box->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	Box->SetStaticMesh(Cube.Object);
	Box->SetHiddenInGame(true);
	
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComp->SetupAttachment(RootComponent);
	ArrowComp->SetArrowColor(FColor::Purple);
	ArrowComp->SetHiddenInGame(false);
	ArrowComp->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));

    for (int i = 0; i < 4; i++)
    {
	    if (!StaticMeshRefs[i])
	    {
	    	UStaticMesh* Mesh = nullptr;

	        if (i == 0)
	        {
	        	const ConstructorHelpers::FObjectFinder<UStaticMesh> FirstCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Base_Full_Train_Car_Base_Full.Train_Car_Base_Full_Train_Car_Base_Full'"));
	        	Mesh = FirstCarMesh.Object;
	        }
	    	else if (i == 1)
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> SecondCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_FlatBed_Full_Train_Car_FlatBed_Full.Train_Car_FlatBed_Full_Train_Car_FlatBed_Full'"));
	    		Mesh = SecondCarMesh.Object;
	        }
	    	else if (i == 2)
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> ThirdCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Weapons_Full_Train_Car_Weapon_Full.Train_Car_Weapons_Full_Train_Car_Weapon_Full'"));
	    		Mesh = ThirdCarMesh.Object;
	        }
	    	else if (i == 3)
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> FourthCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Windows_Full.Train_Car_Windows_Full'"));
	    		Mesh = FourthCarMesh.Object;
	        }

	    	StaticMeshRefs[i] = Mesh;
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
			isTrainMoving = true;
		}
	}, (StartDelayTime >= 1) ? StartDelayTime : 0.1f, false);

	//GEngine->AddOnScreenDebugMessage(2, 5, FColor::Blue, FString::Printf(TEXT("Train Has Started Counting, will begin moving in %d Seconds"), StartDelayTime));

	// Spawning in the Carriages // Doesn't fire if CarriageCount <= 0
	for (int i = 0; i < CarriageCount; i++)
	{
		ATrainCarriage* TempRef = Cast<ATrainCarriage>(GetWorld()->SpawnActor(ATrainCarriage::StaticClass()));
		TempRef->InitialiseFromEngine(i, StaticMeshRefs[i%4], TrackSplineRef);
		
		CarriageRefs.Push(TempRef);
	}

	EngineStart = ((CarriageCount >= 0) ? CarriageCount : 0) * 1500;

	if (AbilitySystemComponent)
	{
		AddStartupGameplayAbilities();
	}

	if (!StopIndices.IsEmpty())
	{
		for (int i = 0; i < StopIndices.Num(); ++i)
		{
			StoppedAtIndex.Push(false);
		}
	}
}

// Called every frame
void ATrainEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Standard Tick Operation
    if (bHasStartedMoving)
    {
    	if (isTrainMoving)
    	{
    		TimeSinceStart += (DeltaTime * TrainSpeedModifier);
    	}
    	
	    const float TimerTrack = TimeSinceStart / TimeToComplete;

    	const float CurrentSplineProgress = FMath::Lerp(0, SplineLength, TimerTrack);

    	SetActorLocation(TrackSplineRef->GetLocationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World));
    	SetActorRotation(TrackSplineRef->GetRotationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World) - FRotator(0.0f, 90.0f, 0.0f));

        if (!StopIndices.IsEmpty())
        {
	        for (int i = StopIndices.Num() - 1; i >= 0; --i)
	        {
		        if (StopIndices[i] < TrackSplineRef->GetNumberOfSplinePoints() && StopIndices[i] >= 0)
		        {
			        const float IndexDistance = TrackSplineRef->GetDistanceAlongSplineAtSplinePoint(StopIndices[i]);

			        if (CurrentSplineProgress >= IndexDistance)
			        {
				        if (!StoppedAtIndex[i])
				        {
					        StoppedAtIndex[i] = true;

					        ToggleTrainStop();
				        }
			        }
		        }
	        }
        }

        for (int i = 0; i < CarriageRefs.Num(); i++)
        {
	        CarriageRefs[i]->ProcessMovement(CurrentSplineProgress);
        }
    	
    	if (!UKismetMathLibrary::InRange_FloatFloat(TimerTrack, 0.0, 1.0, false, true))
    	{
    		TimeSinceStart = 0;
    	}
    }
}

void ATrainEngine::ToggleTrainStop()
{
	isTrainMoving = !isTrainMoving;
}

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