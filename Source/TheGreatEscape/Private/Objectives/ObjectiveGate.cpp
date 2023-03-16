// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Objectives/ObjectiveGate.h"

#include "SplineTrack.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObjectiveGate::AObjectiveGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Mesh"));
	RootComponent = GateMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	GateMesh->SetStaticMesh(MeshObj.Object);
	GateMesh->SetWorldScale3D(FVector(10.0f, 0.5f, 5.0f));

	PickupItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Item"));
	PickupItem->SetupAttachment(RootComponent);
	PickupItem->SetStaticMesh(MeshObj.Object);
	PickupItem->ComponentTags.Push("Interactable");

	// PickupLocation = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Location"));
	// PickupLocation->SetupAttachment(RootComponent);
	// PickupLocation->SetRelativeLocation(FVector(0.0f, 500.0f, 0.0f));

	TrainDetector = CreateDefaultSubobject<USphereComponent>(TEXT("Train Detector"));
	// TrainDetector->SetupAttachment(GateMesh);
	TrainDetector->SetSphereRadius(1000.0f);
	TrainDetector->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveGate::BeginSphereOverlap);

	// PickupItem = CreateDefaultSubobject<AActor>(TEXT("OOF"));
	// // PickupItem->GetParentComponent()->SetupAttachment(RootComponent);
	// UStaticMeshComponent* ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Actor Mesh"));
	// ActorMesh->SetupAttachment(PickupItem->GetRootComponent());
	// ActorMesh->SetStaticMesh(MeshObj.Object);
	// PickupItem->Tags.Push("Interactable");

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

	// PickupItem->SetActorLocation(PickupLocation->GetComponentLocation());
}

// Called every frame
void AObjectiveGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR
  void AObjectiveGate::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (SplineRef)
	{
		GateMesh->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}

	else if (PickupItem->GetAttachParent() == RootComponent && bFinished)
	{
		PickupItem->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		PickupItem->SetWorldScale3D(FVector(1.0f));
	}
	
	TrainDetector->SetWorldLocation(GetActorLocation());
}

void AObjectiveGate::PostEditUndo()
{
	Super::PostEditUndo();

	if (SplineRef)
	{
		GateMesh->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}

	TrainDetector->SetWorldLocation(GetActorLocation());

}

void AObjectiveGate::SnapRotation() const
{
	if (SplineRef)
	{
		// PickupLocation->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GateMesh->SetWorldRotation(SplineRef->GetSpline()->FindRotationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World) + FRotator(0.0f, 90.0f, 0.0f));
		// PickupLocation->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

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

		// Rest of Objective Stuff
	}
	else if (TrainStopped && OtherComp->ComponentHasTag("Interactable"))
	{
		TrainStopped = false;
		EngineRef->ToggleTrainStop();
	}
}
#endif
