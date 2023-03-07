// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainCarriage.h"

#include "Components/SplineComponent.h"

// Static Variable Declarations
// TStaticArray<int, 4> ATrainCarriage::CarriageDistances;

// Sets default values
ATrainCarriage::ATrainCarriage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Carriage Mesh"));
	Box->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	Box->SetStaticMesh(MeshObj.Object);
	Box->SetWorldScale3D(FVector(2.5f, 1.0f, 0.7f));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetArrowColor(FColor::Blue);
	Arrow->SetHiddenInGame(false);
	Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
}

void ATrainCarriage::InitialiseFromEngine(int CarriageNum, UStaticMesh* AssignedMesh, USplineComponent* NewSplineRef)
{
	CarriageNumber = CarriageNum;
	
	Box->SetStaticMesh(AssignedMesh);
	Box->SetWorldScale3D(FVector(1.0f));

	DistanceFromFront = 1500 * (CarriageNumber + 1);

	if (!SplineRef)
	{
		SplineRef = NewSplineRef;
	}
}

// Called when the game starts or when spawned
void ATrainCarriage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrainCarriage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrainCarriage::ProcessMovement(float EngineSplineDist)
{
	float CarriageDist = EngineSplineDist - DistanceFromFront;

	if (CarriageDist < 0)
	{
		float Temp = CarriageDist;
		CarriageDist = SplineRef->GetSplineLength() + Temp;
	}
	
	SetActorLocation(SplineRef->GetLocationAtDistanceAlongSpline(CarriageDist, ESplineCoordinateSpace::World));
	SetActorRotation(SplineRef->GetRotationAtDistanceAlongSpline(CarriageDist, ESplineCoordinateSpace::World) + FRotator(0.0f, 90.0f, 0.0f));
}

