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

#include "Character/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "TrainEngine.h"
#include "Components/SplineComponent.h"
#include "Engine/Rectlight.h"

// Static Variable Declarations
// TStaticArray<int, 4> ATrainCarriage::CarriageDistances;
ATrainEngine* ATrainCarriage::EngineRef;
// APlayerCharacter* ATrainCarriage::PlayerRef;

/**
 * @brief
 * Sets default values for the TrainCarriage. This object is typically only spawned in by the engine.
 */
ATrainCarriage::ATrainCarriage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	PlayerDetectionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Detector"));
	PlayerDetectionComp->SetupAttachment(RootComponent);
	PlayerDetectionComp->InitBoxExtent(FVector(200.0f, 900.0f, 350.0f));
	PlayerDetectionComp->SetRelativeLocation(FVector(0.0f, 0.0f, 450.0f));
	// PlayerDetectionComp->SetHiddenInGame(false);
	PlayerDetectionComp->OnComponentBeginOverlap.AddDynamic(this, &ATrainCarriage::BeginCarriageOverlap);
	PlayerDetectionComp->OnComponentEndOverlap.AddDynamic(this, &ATrainCarriage::EndCarriageOverlap);
}

/**
 * @brief
 * An initialiser function that is intended to be called from the engine.
 * Populates the carriage's Number, Static Mesh, and Spline Reference.
 * Must be called when initialising each carriage
 * @param CarriageNum The number of carriage this is relative to the engine.
 * @param InitDistanceFromFront The distance behind the engine that the carriage is set to be
 * @param CarriageMeshClass The train carriage mesh being assigned to this carriage.
 * @param NewSplineRef The reference to the spline that the engine is using to travel
 * @param NewEngineRef
 */
void ATrainCarriage::InitialiseFromEngine(
	int CarriageNum,
	int InitDistanceFromFront,
	const TSubclassOf<AActor> CarriageMeshClass,
	USplineComponent* NewSplineRef,
	ATrainEngine* NewEngineRef)
{
	CarriageNumber = CarriageNum;

	DistanceFromFront = InitDistanceFromFront;

	if (CarriageMeshClass)
	{
		CarriageMeshActor = GetWorld()->SpawnActor<AActor>(CarriageMeshClass);
		CarriageMeshActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	if (!SplineRef)
	{
		SplineRef = NewSplineRef;
	}

	// If the carriage is not a flatbed... (we're about to do lighting stuff and the flatbed is open so it doesn't need lighting)
	if (CarriageNumber % 4 != 1)
	{
		// Create and add two RectLights into the LightRefs TStaticArray<>()
		LightRefs[0] = (Cast<ARectLight>(GetWorld()->SpawnActor(ARectLight::StaticClass())));
		LightRefs[1] = (Cast<ARectLight>(GetWorld()->SpawnActor(ARectLight::StaticClass())));
		
		// Set default settings for the RectLights in the array. It's inside a foreach given they both need the behaviour and it saves Lines of Code (LOC)
		for (ALight* Light : LightRefs)
		{
			Light->SetMobility(EComponentMobility::Movable);
			Light->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			// Given they are all rect lights and need to be rotated
			Light->SetActorRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
		}

		// Setting up some const ish variables for use in positioning
		constexpr int YPosition = 250;
		const int ZPosition = CarriageNumber != 2 ? 550 : 500;

		// Setting the location of each light using the const-ish variables from before
		LightRefs[0]->SetActorRelativeLocation(FVector(0.0f, YPosition, ZPosition));
		LightRefs[1]->SetActorRelativeLocation(FVector(0.0f, -YPosition, ZPosition));

		if (!EngineRef)
		{
			EngineRef = NewEngineRef;
		}
	}

	ProcessMovement(0);
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
		const float Temp = CarriageDist;
		CarriageDist = SplineRef->GetSplineLength() + Temp;
	}
	
	const FVector CurrentSplineVector = SplineRef->GetLocationAtDistanceAlongSpline(CarriageDist, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotator = SplineRef->GetRotationAtDistanceAlongSpline(CarriageDist, ESplineCoordinateSpace::World) - FRotator(0.0f, 90.0f, 0.0f);
	CurrentSplineRotator.Pitch = 0;
	
	SetActorLocation(CurrentSplineVector);
	SetActorRotation(CurrentSplineRotator);
}

UBoxComponent* ATrainCarriage::GetPlayerDetectionComponent() const
{
	return PlayerDetectionComp;
}

void ATrainCarriage::BeginCarriageOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (EngineRef && OtherActor == EngineRef->PlayerRef)
	{
		EngineRef->BeginEngineOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

void ATrainCarriage::EndCarriageOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (EngineRef && OtherActor == EngineRef->PlayerRef)
	{
		EngineRef->EndEngineOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	}
}
