// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveManager.cpp
// Description : Contains the declarations and definitions for the ObjectiveManager c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "Objectives/ObjectiveManager.h"

#include "SplineTrack.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Objectives/ObjectiveDefense.h"
#include "Objectives/ObjectiveParent.h"
#include "Objectives/ObjectivePickup.h"
#include "Objectives/ObjectiveSecure.h"

/**
 * @brief Sets default values
 */
AObjectiveManager::AObjectiveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// Disabled to improve performance
	PrimaryActorTick.bCanEverTick = false;

	// Populate and set up the static mesh component
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	StaticMesh->SetStaticMesh(MeshObj.Object);
}

/**
 * @brief Called when the game starts or when spawned
 */
void AObjectiveManager::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * @brief Called every frame. Disabled in the constructor
 * @param DeltaTime The amount of time between frames
 */
void AObjectiveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * @brief Creates a Pickup Objective Type and adds it to the objective array
 */
void AObjectiveManager::AddPickupObjective()
{
	// Checks if the spline has been populated and that we haven't added too many objectives
	if (!ReachedCap() && SplineRef)
	{
		// Create the objective and immediately push it onto the objectives array
		ObjectiveRefs.Push(Cast<AObjectivePickup>(GetWorld()->SpawnActor(AObjectivePickup::StaticClass())));
		// Update the position of every objective now that a new objective has been added
		UpdateArray();
	}
}

/**
 * @brief Creates a Defense Objective Type and adds it to the objective array
 */
void AObjectiveManager::AddDefenseObjective()
{
	// Checks if the spline has been populated and that we haven't added too many objectives
	if (!ReachedCap() && SplineRef)
	{
		// Create the objective and immediately push it onto the objectives array
		ObjectiveRefs.Push(Cast<AObjectiveDefense>(GetWorld()->SpawnActor(AObjectiveDefense::StaticClass())));
		// Update the position of every objective now that a new objective has been added
		UpdateArray();
	}
}

/**
 * @brief Creates a Secure Objective Type and adds it to the objective array
 */
void AObjectiveManager::AddSecureObjective()
{
	// Checks if the spline has been populated and that we haven't added too many objectives
	if (!ReachedCap() && SplineRef)
	{		
		// Create the objective and immediately push it onto the objectives array
		ObjectiveRefs.Push(Cast<AObjectiveSecure>(GetWorld()->SpawnActor(AObjectiveSecure::StaticClass())));
		// Update the position of every objective now that a new objective has been added
		UpdateArray();
	}
}

/**
 * @brief 
 */
void AObjectiveManager::UndoObjective()
{
	// Check to ensure we have an objective to pop
	if (!ObjectiveRefs.IsEmpty())
	{
		// Remove the most recent addition to the objective array
		ObjectiveRefs.Pop();
		// Update the position of every objective now that a new objective has been added
		UpdateArray();
	}
}

/**
 * @brief Destroy all of the objectives inside the objective array and then empty the references
 */
void AObjectiveManager::ClearObjectives()
{
	// Iterate through the array and destroy any objectives with valid references
	for (int i = ObjectiveRefs.Num() - 1; i >= 0; i--)
	{
		// Check if the reference is valid
		if (IsValid(ObjectiveRefs[i]))
		{
			// Destroy the referenced objective
			ObjectiveRefs[i]->Destroy();
		}
	}

	// Empty the array of all references. In theory they're all empty by this point.
	ObjectiveRefs.Empty();
}

/**
 * @brief Update the position of each of the objectives along the spline reference.
 * This is done relative to the number of objectives that exist in the Objectives Array
 */
void AObjectiveManager::UpdateArray()
{
	// Check if the spline reference is populated.
	if (SplineRef)
	{
		// Get the length of a single spline segment relative to the number of objectives in the objectives array.
		const float SplineSegment = SplineRef->GetSpline()->GetSplineLength() / (1 + ObjectiveRefs.Num());

		// Iterate through each of the objectives:
		// populating their spline references,
		// setting their locations and rotations,
		// then ensuring they are set to be children of this manager
		for (int i = 0; i < ObjectiveRefs.Num(); i++)
		{
			// Check if the objective's spline reference has been populated
			if (!ObjectiveRefs[i]->SplineRefPopulated())
			{
				// Populate the objective's spline reference with the same spline the manager has in reference.
				ObjectiveRefs[i]->PopulateSplineRef(SplineRef->GetSpline());
			}

			// Set the location and rotation of each objective using data from a specified location along the spline.
			ObjectiveRefs[i]->SetActorLocation(SplineRef->GetSpline()->GetLocationAtDistanceAlongSpline(SplineSegment * (i + 1), ESplineCoordinateSpace::World));
			ObjectiveRefs[i]->SetActorRotation(SplineRef->GetSpline()->GetRotationAtDistanceAlongSpline(SplineSegment * (i + 1), ESplineCoordinateSpace::World));

			// Check if the objective has this manager set as its parent
			if (!ObjectiveRefs[i]->IsAttachedTo(this))
			{
				// set this manager as the objective's parent.
				ObjectiveRefs[i]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

#if WITH_EDITOR	// This means that this functionality only exists in editor and not in any packages
/**
 * @brief Unreal default function.
 * Called whenever a change happens with a property on an instance of this class.
 * @param TransactionEvent The event that occurred and to what.
 */
void AObjectiveManager::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	Super::PostTransacted(TransactionEvent);

	// Checks if the event was a finalisation, if the spline reference is populated, and if the Objective parent class does not have its spline reference populated
	if (TransactionEvent.GetEventType() == ETransactionObjectEventType::Finalized && SplineRef && !AObjectiveParent::SplineRefPopulated())
	{
		// Populate the spline reference of the objective parent class
		AObjectiveParent::PopulateSplineRef(SplineRef->GetSpline());
	}
}
#endif

