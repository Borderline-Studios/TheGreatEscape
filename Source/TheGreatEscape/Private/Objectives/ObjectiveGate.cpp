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
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief Sets Default Values
 */
AObjectiveGate::AObjectiveGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Populating and setting up the Static mesh. This one gets set up as the root component,
	// Also setting the scale and the static mesh used by the mesh component
	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Mesh"));
	RootComponent = GateMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	GateMesh->SetStaticMesh(MeshObj.Object);
	GateMesh->SetWorldScale3D(FVector(10.0f, 0.5f, 5.0f));

	// Populating and setting up the Sphere Collision Component. Also sets the radius of the sphere.
	TrainDetector = CreateDefaultSubobject<USphereComponent>(TEXT("Train Detector"));
	TrainDetector->SetSphereRadius(1000.0f);
	TrainDetector->SetWorldLocation(GetActorLocation());
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
}

/**
 * @brief Called every frame. This Tick has been disabled as it is not required for this Objective
 * @param DeltaTime The change in time between frames
 */
void AObjectiveGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR	// This means that these functions only exist and apply while in the editor. These do not apply to packages
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
		GateMesh->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}

	// Keeps the Train Detector SphereComponent moving with the parent mesh no matter what.
	TrainDetector->SetWorldLocation(GetActorLocation());
}

/**
 * @brief This Unreal function is called whenever the undo operation is performed in editor on this item.
 *		  Currently sets the location of everything to where it was based on the previous location it occupied.
 */
void AObjectiveGate::PostEditUndo()
{
	Super::PostEditUndo();

	if (SplineRef && bSnapToTrack)
	{
		GateMesh->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}

	TrainDetector->SetWorldLocation(GetActorLocation());
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
		GateMesh->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
		GateMesh->SetWorldRotation(SplineRef->GetSpline()->FindRotationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World) + FRotator(0.0f, 90.0f, 0.0f));

		TrainDetector->SetWorldLocation(GetActorLocation());
	}
}

void AObjectiveGate::SpawnPickup()
{
	AActor* NewPickup = GetWorld()->SpawnActor(PickupItemClassRef);
	NewPickup->SetActorLocation((GetActorLocation() + (GateMesh->GetForwardVector() * 1000.00)) + FVector(0.0f, 0.0f, 100.0f));
	PickupItems.Push(NewPickup);
}

void AObjectiveGate::RemovePickup()
{
	if (!PickupItems.IsEmpty())
	{
		PickupItems.Last()->Destroy();
		PickupItems.Pop();
	}
}

void AObjectiveGate::ClearPickups()
{
	for (AActor* Item : PickupItems)
	{
		Item->Destroy();
	}
	PickupItems.Empty();
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
	// If the object colliding with the sphere is the train engine AND the internal tracking variable says we haven't stopped it yet...
	// Stop the Train and update the tracking boolean
	if (OtherActor == EngineRef && !bTrainStopped)
	{
		bTrainStopped = true;
		EngineRef->ToggleTrainStop();
	}

	// Check to see if the other actor that just collided is any of the pickup interactables that have been spawned
	for (int i = 0; i < PickupItems.Num(); ++i)
	{
		if (OtherActor == PickupItems[i])
		{
			PickupItemPlacedCount++;
			UE_LOG(LogTemp, Warning, TEXT("Number of items Detected: %i"), PickupItemPlacedCount);
			UE_LOG(LogTemp, Warning, TEXT("Number of items Required: %i"), PickupItems.Num());
		}
	}

	// If the train has stopped and the other component has the tag "interactable" then
	// Start the train and update the tracking variable.
	if (bTrainStopped && PickupItemPlacedCount == PickupItems.Num())
	{
		bTrainStopped = false;
		EngineRef->ToggleTrainStop();
	}
}

void AObjectiveGate::EndSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	for (int i = 0; i < PickupItems.Num(); ++i)
	{
		if (OtherActor == PickupItems[i])
		{
			//PickupItemPlacedCount--;
		}
	}
}
