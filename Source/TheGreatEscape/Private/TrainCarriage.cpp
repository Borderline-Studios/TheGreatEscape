// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2023 Media Design School
//
// File Name   :	TrainCarriage.cpp
// Description :	Contains the implementation of the Train Carriage functionality
// Author      :	Borderline Studios - Jake Laird
// Mail        :	jake.laird@mds.ac.nz

#include "TrainCarriage.h"

#include "Components/SplineComponent.h"

// Static Variable Declarations
// TStaticArray<int, 4> ATrainCarriage::CarriageDistances;

/**
 * @brief
 * Sets default values for the TrainCarriage. This object is typically only spawned in by the engine.
 */
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

/**
 * @brief
 * An initialiser function that is intended to be called from the engine.
 * Populates the carriage's Number, Static Mesh, and Spline Reference.
 * Must be called when initialising each carriage
 * @param CarriageNum The number of carriage this is relative to the engine.
 * @param AssignedMesh The train carriage mesh being assigned to this carriage.
 * @param NewSplineRef The reference to the spline that the engine is using to travel
 */
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

/**
 * @brief
 * This is the function that updates the position of the carriage based on the engine's distance along the spline and an offset specific to each carriage.
 * This is called typically inside the tick function of the engine.
 * @param EngineSplineDist The engine's current distance along the spline
 */
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

