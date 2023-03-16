// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   :
// Description :
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        : 

#include "Objectives/ObjectiveManager.h"

#include "SplineTrack.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Objectives/ObjectiveDefense.h"
#include "Objectives/ObjectiveParent.h"
#include "Objectives/ObjectivePickup.h"
#include "Objectives/ObjectiveSecure.h"

// Sets default values
AObjectiveManager::AObjectiveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	StaticMesh->SetStaticMesh(MeshObj.Object);

	
}

// Called when the game starts or when spawned
void AObjectiveManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectiveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectiveManager::AddPickupObjective()
{
	if (!ReachedCap() && SplineRef)
	{
		ObjectiveRefs.Push(Cast<AObjectivePickup>(GetWorld()->SpawnActor(AObjectivePickup::StaticClass())));
		UpdateArray();
	}
}

void AObjectiveManager::AddDefenseObjective()
{
	if (!ReachedCap() && SplineRef)
	{
		ObjectiveRefs.Push(Cast<AObjectiveDefense>(GetWorld()->SpawnActor(AObjectiveDefense::StaticClass())));
		UpdateArray();
	}
}

void AObjectiveManager::AddSecureObjective()
{
	if (!ReachedCap() && SplineRef)
	{		
		ObjectiveRefs.Push(Cast<AObjectiveSecure>(GetWorld()->SpawnActor(AObjectiveSecure::StaticClass())));
		UpdateArray();
	}
}

void AObjectiveManager::UndoObjective()
{
	if (ObjectiveRefs.Num() > 0)
	{
		ObjectiveRefs.Pop();
		UpdateArray();
	}
}

void AObjectiveManager::ClearObjectives()
{
	for (int i = ObjectiveRefs.Num() - 1; i >= 0; i--)
	{
		ObjectiveRefs[i]->Destroy();
	}
	
	ObjectiveRefs.Empty();
}

void AObjectiveManager::UpdateArray()
{
	if (SplineRef)
	{
		const float SplineSegment = SplineRef->GetSpline()->GetSplineLength() / (1 + ObjectiveRefs.Num());
		
		for (int i = 0; i < ObjectiveRefs.Num(); i++)
		{
			if (!ObjectiveRefs[i]->SplineRefPopulated())
			{
				ObjectiveRefs[i]->PopulateSplineRef(SplineRef->GetSpline());
			}
			
			ObjectiveRefs[i]->SetActorLocation(SplineRef->GetSpline()->GetLocationAtDistanceAlongSpline(SplineSegment * (i + 1), ESplineCoordinateSpace::World));
			ObjectiveRefs[i]->SetActorRotation(SplineRef->GetSpline()->GetRotationAtDistanceAlongSpline(SplineSegment * (i + 1), ESplineCoordinateSpace::World));

			if (!ObjectiveRefs[i]->IsAttachedTo(this))
			{
				ObjectiveRefs[i]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

#if WITH_EDITOR
  void AObjectiveManager::PostTransacted(const FTransactionObjectEvent& TransactionEvent)
{
	Super::PostTransacted(TransactionEvent);

	if (TransactionEvent.GetEventType() == ETransactionObjectEventType::Finalized && SplineRef && !AObjectiveParent::SplineRefPopulated())
	{
		AObjectiveParent::PopulateSplineRef(SplineRef->GetSpline());
	}
}
#endif

