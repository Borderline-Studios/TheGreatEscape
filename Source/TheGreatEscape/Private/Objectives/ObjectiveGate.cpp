// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   :
// Description :
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        : 

#include "Objectives/ObjectiveGate.h"

#include "SplineTrack.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief Sets Default Values
 */
AObjectiveGate::AObjectiveGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	// Populating and setting up the Static mesh. This one gets set up as the root component,
	// Also setting the scale and the static mesh used by the mesh component
	GateFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Frame"));
	GateFrame->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> GateFrameMesh(TEXT("StaticMesh'/Game/Production/Enviroment/Interactables/Gate/Gate_GateFrame.Gate_GateFrame'"));
	GateFrame->SetStaticMesh(GateFrameMesh.Object);
	GateFrame->SetRelativeLocation(FVector(-450.0f, 0.0f, 0.0f));
	
	GateRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Right"));
	GateRight->SetupAttachment(GateFrame);
	ConstructorHelpers::FObjectFinder<UStaticMesh> GateRightMesh(TEXT("StaticMesh'/Game/Production/Enviroment/Interactables/Gate/Gate_GateLeft.Gate_GateLeft'"));
	GateRight->SetStaticMesh(GateRightMesh.Object);
	RightGateRelativeLocation = FVector(300.0f, 0.0f, 0.0f);
	GateRight->SetRelativeLocation(RightGateRelativeLocation);
	
	GateLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Left"));
	GateLeft->SetupAttachment(GateFrame);
	ConstructorHelpers::FObjectFinder<UStaticMesh> GateLeftMesh(TEXT("StaticMesh'/Game/Production/Enviroment/Interactables/Gate/Gate_GateRight.Gate_GateRight'"));
	GateLeft->SetStaticMesh(GateLeftMesh.Object);
	LeftGateRelativeLocation = FVector(-250.0f, 0.0f, 0.0f);
	GateLeft->SetRelativeLocation(LeftGateRelativeLocation);

	// Populating and setting up the Sphere Collision Component. Also sets the radius of the sphere.
	TrainDetector = CreateDefaultSubobject<USphereComponent>(TEXT("Train Detector"));
	TrainDetector->SetupAttachment(RootComponent);
	TrainDetector->SetSphereRadius(2000.0f);
	// TrainDetector->SetWorldLocation(GetActorLocation());
	// The lines below assign the BeginSphereOverlap and EndSphereOverlap function to act as its OnComponentBeginOverlap and OnComponentEndOverlap function delegates.
	TrainDetector->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveGate::BeginSphereOverlap);
	TrainDetector->OnComponentEndOverlap.AddDynamic(this, &AObjectiveGate::EndSphereOverlap);

	// Populating the soft class pointer so that proper assets can be spawned.
	PickupItemClassRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Objectives/PickUp/BP_PickUpObjective.BP_PickUpObjective_C'"))).LoadSynchronous();

	EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
}

/**
 * @brief Called when the game starts or when spawned
 */
void AObjectiveGate::BeginPlay()
{
	Super::BeginPlay();

	if (!EngineRef)
	{
		EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
	}

	PickupItemPlacedCount = 0;
	PickupItemsNum = PickupItems.Num();
	SetActorTickEnabled(false);

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass()));
	}
}

/**
 * @brief Called every frame. Starts disabled for the actor, is switched on when the door is opened and then disabled again once the door has opened
 * @param DeltaTime The change in time between frames
 */
void AObjectiveGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceEnabled += DeltaTime;

	if (TimeSinceEnabled < TimeTakenToOpen)
	{
		const float AlphaTime = TimeSinceEnabled / TimeTakenToOpen;
		const FVector CurrentRightPos = FMath::Lerp(RightGateRelativeLocation, RightGateRelativeLocation + FVector(DoorMoveDistance, 0.0f, 0.0f), AlphaTime);
		const FVector CurrentLeftPos = FMath::Lerp(LeftGateRelativeLocation, LeftGateRelativeLocation + FVector(-DoorMoveDistance, 0.0f, 0.0f), AlphaTime);

		GateRight->SetRelativeLocation(CurrentRightPos);
		GateLeft->SetRelativeLocation(CurrentLeftPos);
	}
	else if (TimeSinceEnabled >= TimeTakenToOpen)
	{
		bTrainStopped = false;
		EngineRef->EnableMovement();

		// Default call of this function sets the text back to ""
		UpdateObjectiveText();
		
		TimeSinceEnabled = 0;
		SetActorTickEnabled(false);
	}

}

void AObjectiveGate::UpdateObjectiveText(const FString NewText) const
{
	if (EngineRef)
	{
		EngineRef->UpdateObjectiveText(NewText);
	}
}

#if WITH_EDITOR	// This means that these functions only exist and apply while in the editor. These do not apply to packages
/**
 * @brief 
 */
void AObjectiveGate::Destroyed()
{
	Super::Destroyed();

	ClearPickups();
}

/**
 * @brief This Unreal function is called while an asset is being moved in editor.
 *		  The purpose of this one is to keep all of the separate components moving together as the SphereComponent isn't attached to the rootcomponent
 *		  It also makes the mesh move along the spline whe moving provided the required conditions are met
 * @param bFinished Whether or not the move action has completed. Allows for separation of functionality.
 */
void AObjectiveGate::PostEditMove(bool bFinished)
{
	// Calls the parent's version of this function. Was present during function generation
	Super::PostEditMove(bFinished);

	// Checks to see if the PickupItem is still connected to the RootComponent which should only be when this is first dragged into scene.
	// Provided these conditions are met, it disconnects the PickupItem from the rest of the actor so that it doesn't move or rotate with the RootComponent
	// if (PickupItem->GetAttachParent() == RootComponent && bFinished)
	// {
	// 	PickupItem->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// 	PickupItem->SetWorldScale3D(FVector(1.0f));
	// }

	// Checks to see if the spline ref is populated and if we want to snap to the track.
	// If both are true (if the spline ref is populated and we want to snap to track) then the mesh will snap to the nearest point on the spline from where it's located in scene.
	if (SplineRef && bSnapToTrack)
	{
		Root->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}
}

/**
 * @brief This Unreal function is called whenever the undo operation is performed in editor on this item.
 *		  Currently sets the location of everything to where it was based on the previous location it occupied.
 */
void AObjectiveGate::PostEditUndo()
{
	Super::PostEditUndo();

	CleanPickupsArray();

	if (SplineRef && bSnapToTrack)
	{
		Root->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}
}

/**
 * @brief This custom function is only to exist in editor because that is the only place it will be used.
 *		  Sets the Location and Rotation of the spline to that of the point nearest it's location at the time of this function being called.
 *		  Only occurs if the conditions of the Spline reference being populated and the boolean being set to true in editor
 */
void AObjectiveGate::SnapRotation() const
{
	if (SplineRef && bSnapToTrack)
	{
		Root->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
		Root->SetWorldRotation(SplineRef->GetSpline()->FindRotationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World) + FRotator(0.0f, -90.0f, 0.0f));
	}
}

/**
 * @brief 
 */
void AObjectiveGate::SpawnPickup()
{
	CleanPickupsArray();
	
	AActor* NewPickup = GetWorld()->SpawnActor(PickupItemClassRef);
	NewPickup->SetActorLocation((GetActorLocation() + (GateFrame->GetRightVector() * (-250.00 - (50 * PickupItems.Num())))) + FVector(0.0f, 0.0f, 100.0f));
	NewPickup->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	PickupItems.Push(NewPickup);
}

/**
 * @brief 
 */
void AObjectiveGate::RemovePickup()
{
	if (CleanPickupsArray()) return;		

	if (!PickupItems.IsEmpty() && IsValid(PickupItems.Last()))
	{
		PickupItems.Last()->Destroy();
		PickupItems.Pop();
	}
}

/**
 * @brief 
 */
void AObjectiveGate::ClearPickups()
{
	for (int i = 0; i < PickupItems.Num(); i++)
	{
		if (PickupItems[i])
		{
			PickupItems[i]->Destroy();
		}
	}
	
	PickupItems.Empty();
}

void AObjectiveGate::FixReferences()
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	CleanPickupsArray();

	for (int i = 0; i < AttachedActors.Num(); ++i)
	{
		if (AttachedActors[i]->GetClass() == PickupItemClassRef)
		{
			PickupItems.Push(AttachedActors[i]);
		}
	}
}

bool AObjectiveGate::CleanPickupsArray()
{
	bool bCleanedUp = false;
	
	for (int i = PickupItems.Num() - 1; i >= 0; i--)
	{
		if (!IsValid(PickupItems[i]))
		{
			PickupItems.RemoveAt(i);

			if (!bCleanedUp)
			{
				bCleanedUp = true;
				
			}
		}
	}

	return bCleanedUp;
}
#endif

/**
 * @brief Default Unreal function that allows for the handling of collisions for specific objects. The specific objects are set separately, usually inside the constructor.
 *		  This instance handles two collision instances:
 *		  - The event that the train is colliding with the Train Detector Sphere Component.
 *			-> Sets the train to stop and sets the internal tracking boolean of bTrainStopped to true so that it doesn't repeat.
 *		  - The event that the PickupItem is brought into the sphere.
 *			-> If the Train has been stopped, it allows for the train to start back up again.
 * @param OverlappedComponent 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 * @param bFromSweep 
 * @param SweepResult 
 */
void AObjectiveGate::BeginSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!bOpened)
	{
		// If the object colliding with the sphere is the train engine AND the internal tracking variable says we haven't stopped it yet...
		// Stop the Train and update the tracking boolean
		if (OtherActor == EngineRef && OtherComp == EngineRef->GetEngineDetectionComponent() && !bTrainStopped)
		{
			bTrainStopped = true;
			EngineRef->ToggleTrainStop();
			EngineRef->DisableMovement();
			FString ObjText = "Collect ";
			ObjText.AppendInt(PickupItemsNum);
			ObjText.Append(((PickupItemsNum == 1) ? " Battery." : " Batteries."));
			ObjText.Append(" Explore nearby!");
			UpdateObjectiveText(ObjText);
		}

		// Check to see if the train is stopped
		if (bTrainStopped && OtherActor == PlayerRef)
		{
			// Check to see if the other actor that just collided is any of the pickup interactables that have been spawned
			for (int i = 0; i < PickupItems.Num(); i++)
			{
				if (PlayerRef->bBatteryPickedUp)
				{
					PlayerRef->bBatteryPickedUp = false;
				
					PickupItemPlacedCount++;
				
					UE_LOG(LogTemp, Warning, TEXT("Number of items Detected: %i"), PickupItemPlacedCount);
					UE_LOG(LogTemp, Warning, TEXT("Number of items Required: %i"), PickupItems.Num());

					PickupItems.RemoveAt(i);

					if (PickupItems.Num() != 0)
					{
						const int RemainingPickupsToCollect = PickupItemsNum - PickupItemPlacedCount;
						FString ObjectiveText;
						ObjectiveText.AppendInt(RemainingPickupsToCollect);
						ObjectiveText.Append(RemainingPickupsToCollect == 1 ? " battery" : " batteries");
						ObjectiveText.Append(" left to collect!");
						UpdateObjectiveText(ObjectiveText);
					}
				}
			}
		}

		// If the train has stopped and the other component has the tag "interactable" then
		// Start the train and update the tracking variable.
		if (PickupItemPlacedCount == PickupItemsNum)
		{
			bOpened = true;
			SetActorTickEnabled(true);
		
			// Call sound for gate
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GateSFX, GetActorLocation(), FRotator(0,0,0), 1.0f);
		}
	}
}

/**
 * @brief 
 * @param OverlappedComponent 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 */
void AObjectiveGate::EndSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	// for (int i = 0; i < PickupItems.Num(); ++i)
	// {
	// 	if (OtherActor == PickupItems[i])
	// 	{
	// 		//PickupItemPlacedCount--;
	// 	}
	// }
}
