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

// Sets default values
AObjectiveGate::AObjectiveGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Mesh"));
	RootComponent = GateMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	GateMesh->SetStaticMesh(MeshObj.Object);
	GateMesh->SetWorldScale3D(FVector(10.0f, 0.5f, 5.0f));

	PickupItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Item"));
	PickupItem->SetupAttachment(RootComponent);
	PickupItem->SetStaticMesh(MeshObj.Object);
	PickupItem->ComponentTags.Push("Interactable");

	TrainDetector = CreateDefaultSubobject<USphereComponent>(TEXT("Train Detector"));
	// TrainDetector->SetupAttachment(GateMesh);
	TrainDetector->SetSphereRadius(1000.0f);
	TrainDetector->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveGate::BeginSphereOverlap);

	EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
}

// Called when the game starts or when spawned
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

#if WITH_EDITOR
  void AObjectiveGate::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	// Happens once
	if (PickupItem->GetAttachParent() == RootComponent && bFinished)
	{
		PickupItem->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		PickupItem->SetWorldScale3D(FVector(1.0f));
	}
	
	if (SplineRef && bSnapToTrack)
	{
		GateMesh->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}
	
	TrainDetector->SetWorldLocation(GetActorLocation());
}

void AObjectiveGate::PostEditUndo()
{
	Super::PostEditUndo();

	if (SplineRef && bSnapToTrack)
	{
		GateMesh->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}

	TrainDetector->SetWorldLocation(GetActorLocation());
}

void AObjectiveGate::SnapRotation() const
{
	if (SplineRef && bSnapToTrack)
	{
		GateMesh->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
		GateMesh->SetWorldRotation(SplineRef->GetSpline()->FindRotationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World) + FRotator(0.0f, 90.0f, 0.0f));
	}
}
#endif

void AObjectiveGate::BeginSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == EngineRef && !TrainStopped)
	{
		TrainStopped = true;
		EngineRef->ToggleTrainStop();
	}
	else if (TrainStopped && OtherComp->ComponentHasTag("Interactable"))
	{
		TrainStopped = false;
		EngineRef->ToggleTrainStop();
	}
}