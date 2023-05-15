// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveGate.cpp
// Description : Contains the implementation of the ObjectiveGate c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "Objectives/ObjectiveGate.h"

#include "TrainEngine.h"
#include "SplineTrack.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objectives/ObjectiveGateBatterySlot.h"
#include "Objectives/ObjectiveElevator.h"

/**
 * @brief Sets default values
 */
AObjectiveGate::AObjectiveGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Populate and set up the root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Populate and set up the gate meshes
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

	// Populate and set up the Sphere Collision Component. Includes function delegate setup.
	// Also sets the radius of the sphere.
	TrainDetector = CreateDefaultSubobject<USphereComponent>(TEXT("Train Detector"));
	TrainDetector->SetupAttachment(RootComponent);
	TrainDetector->SetSphereRadius(1000.0f);
	// TrainDetector->SetWorldLocation(GetActorLocation());
	TrainDetector->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveGate::BeginTrainDetectorOverlap);
	
	// Populate the class references so that proper assets can be spawned.
	SlotClassRef = AObjectiveGateBatterySlot::StaticClass();
	PickupItemClassRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Objectives/PickUp/BP_PickUpObjective.BP_PickUpObjective_C'"))).LoadSynchronous();

	// Populate the engine reference
	EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
}

/**
 * @brief Called when the game starts or when spawned
 */
void AObjectiveGate::BeginPlay()
{
	Super::BeginPlay();

	// Ensure that the gate knows about its children and the components it has to track
	FixReferences();

	// If the engine reference wasn't populated during construction then it tries to grab it here.
	if (!EngineRef)
	{
		EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
	}

	// Disable tick. This gate only needs to open once so tick is used instead of a timeline
	SetActorTickEnabled(false);

	// Populate the player reference
	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass()));
	}

	// References should have been fixed at this point.
	// Initialise each slot so that it knows that this gate is the one it needs to keep updated
	// and so that it knows what to spawn when the player brings a battery through.
	for (int i = 0; i < SlotRefs.Num(); i++)
	{
		Cast<AObjectiveGateBatterySlot>(SlotRefs[i])->InitialiseFromGate(this, PickupItemClassRef);
	}
}

/**
 * @brief Called every frame. Starts disabled for the actor, is switched on when the door is opened and then disabled again once the door has opened
 * @param DeltaTime The change in time between frames
 */
void AObjectiveGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Increment time
	TimeSinceEnabled += DeltaTime;

	// Check to see if the gate is closing or has closed
	if (TimeSinceEnabled < GateSFX->GetDuration())
	{
		// Calculate an alpha using the time since start and the total time of the move
		// Then use that to AlphaTime to determine how far along the LERPs each door should be
		const float AlphaTime = TimeSinceEnabled / GateSFX->GetDuration();
		const FVector CurrentRightPos = FMath::Lerp(RightGateRelativeLocation, RightGateRelativeLocation + FVector(DoorMoveDistance, 0.0f, 0.0f), AlphaTime);
		const FVector CurrentLeftPos = FMath::Lerp(LeftGateRelativeLocation, LeftGateRelativeLocation + FVector(-DoorMoveDistance, 0.0f, 0.0f), AlphaTime);

		// Using the positions calculated above, set the doors to those locations
		GateRight->SetRelativeLocation(CurrentRightPos);
		GateLeft->SetRelativeLocation(CurrentLeftPos);
	}
	else if (TimeSinceEnabled >= GateSFX->GetDuration())
	{
		bTrainStopped = false;
		EngineRef->EnableMovement();

		// Default call of this function sets the text back to ""
		UpdateObjectiveText();
		
		TimeSinceEnabled = 0;
		SetActorTickEnabled(false);
	}

}

/**
 * @brief Uses the Engine Reference to update the player's objective text.
 * @param NewText The text to change the objective text to. Defaults to ""
 */
void AObjectiveGate::UpdateObjectiveText(const FString NewText) const
{
	if (EngineRef)
	{
		EngineRef->UpdateObjectiveText(NewText);
	}
}

#if WITH_EDITOR	// This means that these functions only exist and apply while in the editor. These do not apply to packages
/**
 * @brief Calls the ClearPickups() function to properly destroy all attached components
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

	// Ensure the slot references are populated
	CleanSlotsArray();

	// Snaps the gate to the track's location where it was at the end of the undo call,
	// Provided a spline reference has been populated and the gate has been set to snap to it.
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
	// Gets the point of the spline closest to the location of the gate
	// then grabs the world location and rotation at that point and applies it to the gate
	if (SplineRef && bSnapToTrack)
	{
		Root->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
		Root->SetWorldRotation(SplineRef->GetSpline()->FindRotationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World) + FRotator(0.0f, -90.0f, 0.0f));
	}
}

/**
 * @brief Spawns a pickup, initialises it, sets it as a child of the root
 * and then adds it to the slot references array.
 */
void AObjectiveGate::SpawnPickup()
{
	// Ensure references are properly populated before adding one more
	FixReferences();

	// Spawn the slot
	AActor* NewSlot = GetWorld()->SpawnActor(SlotClassRef);
	// Set the location and rotation of the slot using the number of populated slot references
	NewSlot->SetActorLocation((GetActorLocation() + (GateFrame->GetRightVector() * (-250.00 - (200 * SlotRefs.Num())))) + FVector(0.0f, 0.0f, 100.0f));
	NewSlot->SetActorRotation(GetActorRotation());
	// Attach the slot as a child of the root.
	NewSlot->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	// Make the slot spawn a battery using its own spawning function
	Cast<AObjectiveGateBatterySlot>(NewSlot)->SpawnPickup(PickupItemClassRef);

	// Add the slot to the array of slot references.
	SlotRefs.Push(NewSlot);
}

/**
 * @brief Removes the most recently added slot reference if one exists.
 * Has a check in it to clean the slots array before removing anything.
 */
void AObjectiveGate::RemovePickup()
{
	// Clean the array. If it does, we return out
	// so that we don't accidentally delete something we didn't want to.
	if (CleanSlotsArray()) return;		

	// Check to ensure there are references inside the array.
	// No point deleting something that doesn't exist
	// Also makes sure the last reference is valid. This should be done by CleanSlotsArray()
	// but it's here just in case something slips through
	if (!SlotRefs.IsEmpty() && IsValid(SlotRefs.Last()))
	{
		SlotRefs.Last()->Destroy();
		SlotRefs.Pop();
	}
}

/**
 * @brief Deletes the entire array of slot references.
 */
void AObjectiveGate::ClearPickups()
{
	// Iterate through the entire array
	for (int i = 0; i < SlotRefs.Num(); i++)
	{
		// Check to ensure the reference is valid
		if (IsValid(SlotRefs[i]))
		{
			SlotRefs[i]->Destroy();
		}
	}

	// Empties the array regardless of whether the references were valid
	SlotRefs.Empty();
}
#endif

/**
 * @brief Reconnects broken references between the gate and its slots
 */
void AObjectiveGate::FixReferences()
{
	// Remove any empty references
	CleanSlotsArray();

	// Obtain the actors attached to the gate not including grandchildren
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	// Iterate through the array, checking if the referenced actor is a slot.
	// If so, add it using the AddUnique function of TArray<>() so that it only adds references that aren't already present
	for (int i = 0; i < AttachedActors.Num(); i++)
	{
		if (AObjectiveGateBatterySlot* CastActor = Cast<AObjectiveGateBatterySlot>(AttachedActors[i]))
		{
			SlotRefs.AddUnique(CastActor);
		}
	}
}

/**
 * @brief Clamp for battery requirements for any elevators attached to the gate.
 * Was intended to function with references instead of copies but Unreal Blueprinting didn't agree with it
 * @param RequirementToAdjust Number of batteries that a given elevator is expecting before it'll start
 * @return A value of RequiredToAdjust that a gate can feasibly achieve
 */
int AObjectiveGate::ElevatorInformationCheck(int RequirementToAdjust)
{
	// Ensure slot references are accurate.
	FixReferences();

	// Determine if the number of batteries the elevator is expecting is possible for the gate.
	const bool bValueInRange = UKismetMathLibrary::InRange_IntInt(RequirementToAdjust, 0, SlotRefs.Num(), true, false);

	// If the elevator is asking for more batteries than can be provided
	// and the array has more than two batteries,
	// set the value to one less than the max amount
	if (!bValueInRange && SlotRefs.Num() >= 2)
	{
		RequirementToAdjust = SlotRefs.Num() - 1;
	}
	// If the above condition was not met, set it to 0
	else
	{
		RequirementToAdjust = 0;
	}

	return RequirementToAdjust;
}

/**
 * @brief Goes through the array and removes any invalid references
 * @return Whether or not any references were removed
 */
bool AObjectiveGate::CleanSlotsArray()
{
	bool bCleanedUp = false;
	
	for (int i = SlotRefs.Num() - 1; i >= 0; i--)
	{
		if (!IsValid(SlotRefs[i]))
		{
			SlotRefs.RemoveAt(i);

			if (!bCleanedUp)
			{
				bCleanedUp = true;
			}
		}
	}

	return bCleanedUp;
}

/**
 * @brief OnComponentBeginOverlap delegate function,
 * used to detect if the train has entered the train detector.
 * @param OverlappedComponent 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 * @param bFromSweep 
 * @param SweepResult 
 */
void AObjectiveGate::BeginTrainDetectorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Check if the gate has been opened yet
	if (!bOpened)
	{
		// If the object colliding with the sphere is the train engine AND the internal tracking variable says we haven't stopped it yet...
		// Stop the Train and update the tracking boolean
		if (OtherActor == EngineRef && OtherComp == EngineRef->GetEngineDetectionComponent() && !bTrainStopped)
		{
			// Update the tracking variable
			bTrainStopped = true;
			// Stop the train
			EngineRef->ToggleTrainStop();
			// Prevent the train from restarting with the lever
			EngineRef->DisableMovement();
			// Populate the objective text using the number of battery slots this gate has
			FString ObjText = "Collect ";
			ObjText.AppendInt(SlotRefs.Num());
			ObjText.Append(((SlotRefs.Num() == 1) ? " Battery." : " Batteries."));
			ObjText.Append(" Explore nearby!");
			UpdateObjectiveText(ObjText);
		}
	}
}

/**
 * @brief Checks to see if all of the slots have been filled or not.
 * Called whenever a slot is provided its battery.
 */
void AObjectiveGate::UpdateFromSlot()
{
	// Check if the gate is opened.
	if (!bOpened)
	{
		// Initialise a counting variable
		int SlotsFilled = 0;

		// Ensure the references in the array are valid
		FixReferences();

		// Iterate through the array and get whether each slot has been filled or not,
		// incrementing the counting variable for each filled slot
		for (int i = 0; i < SlotRefs.Num(); i++)
		{
			if (Cast<AObjectiveGateBatterySlot>(SlotRefs[i])->GetSlotFilled())
			{
				SlotsFilled++;
			}
		}

		// If the elevator reference is populated,
		// see if we can start it using the current number of filled slots 
		if (ElevatorRef)
		{
			ElevatorRef->EnableElevator(SlotsFilled);
		}

		// Checks to see if the number of filled slots matches the number of slots.
		// This indicates whether the gate has had all of its slots filled or not.
		// If not, we return out.
		if (SlotsFilled != (SlotRefs.Num()))
		{
			return;
		}

		// If we do have every slot filled, start the process to open the gate
		bOpened = true;
		SetActorTickEnabled(true);

		// Play the sound of the gate opening
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GateSFX, GetActorLocation(), FRotator(), 1.0f);
	}
}
