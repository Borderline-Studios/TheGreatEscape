// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Train/TrainCarParent.h"
#include "TrainEngine.h"

// PUBLIC
// Sets default values
ATrainCarParent::ATrainCarParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Carriage Mesh"));
	CarMesh->SetupAttachment(RootComponent);

	PlayerDetectionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerDetector"));
	PlayerDetectionComponent->SetupAttachment(RootComponent);
	PlayerDetectionComponent->SetHiddenInGame(true);
	PlayerDetectionComponent->SetVisibility(true);
	// Player Detection Component sets its collision dynamics inside each of the children
}

// PROTECTED
// Called when the game starts or when spawned
void ATrainCarParent::BeginPlay()
{
	Super::BeginPlay();
}

void ATrainCarParent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clear and reset static variables
	ClearStaticTimers();
	
	if (!PlayerDetectionBoxes.IsEmpty())
	{
		PlayerDetectionBoxes.Empty();
	}

	// Clear individual variables
	if (EngineRef)
	{
		EngineRef = nullptr;
	}


	// FINAL (MUST BE LAST)
	PlayerRef = nullptr;
	bPlayerOnTrain = false;
	bClearedStaticTimers = false;
	CurrentWorldRef = nullptr;
}

void ATrainCarParent::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PushPlayerDetectionComponent(PlayerDetectionComponent);
}

// PUBLIC
// Called every frame
void ATrainCarParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// PRIVATE
void ATrainCarParent::ClearStaticTimers()
{
	if (!bClearedStaticTimers && CurrentWorldRef)
	{
		bClearedStaticTimers = true;

		CurrentWorldRef->GetTimerManager().ClearTimer(PlayerDetectionTimerHandle);
	}
}

bool ATrainCarParent::CheckTrainForPlayer() const
{
	for (int i = 0; i < PlayerDetectionBoxes.Num(); i++)
	{
		if (PlayerDetectionBoxes[i]->GetName() != "PlayerDetector")
		{
			PlayerDetectionBoxes.RemoveAt(i);
			i--;
			continue;
		}
		
		TArray<AActor*> OverlappingActors;
		PlayerDetectionBoxes[i]->GetOverlappingActors(OverlappingActors, APawn::StaticClass());

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

// PROTECTED
void ATrainCarParent::BeginCarOverlap(
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
	}
}

void ATrainCarParent::EndCarOverlap(
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

void ATrainCarParent::SetCurrentWorld(UWorld* NewWorldRef)
{
	if (!CurrentWorldRef)
	{
		CurrentWorldRef = NewWorldRef;
	}
}

bool ATrainCarParent::GetPlayerOnTrain()
{
	return bPlayerOnTrain;
}

void ATrainCarParent::PushPlayerDetectionComponent(UBoxComponent* NewCollisionRef)
{
	PlayerDetectionBoxes.Push(NewCollisionRef);
}

void ATrainCarParent::EnableTrainMovementTimer() const
{
	GetWorldTimerManager().SetTimer(PlayerDetectionTimerHandle, [&]()
	{
		if (CheckTrainForPlayer())
		{
			bPlayerOnTrain = true;
		}
	}, 0.75f, false);
}

void ATrainCarParent::DisableTrainMovementTimer() const
{
	GetWorldTimerManager().SetTimer(PlayerDetectionTimerHandle, [&]()
	{
		if (!CheckTrainForPlayer())
		{
			bPlayerOnTrain = false;
			if (EngineRef && EngineRef->GetTrainMoving())
			{
				EngineRef->ToggleTrainStop();
			}
			GetWorldTimerManager().ClearTimer(PlayerDetectionTimerHandle);
		}
	}, 2.0f, true);
}
