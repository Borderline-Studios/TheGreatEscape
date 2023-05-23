// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveParent.cpp
// Description : Contains the implementation of the ObjectiveParent c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "Objectives/ObjectiveParent.h"

#include "Character/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Static Variable Declarations
USplineComponent* AObjectiveParent::SplineRef;

/**
 * @brief Sets default values
 */
AObjectiveParent::AObjectiveParent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Checks if the spline reference has been populated
	if (!SplineRef)
	{
		// Logs a message indicating a missing reference
		UE_LOG(LogTemp, Warning, TEXT("Missing a Spline Reference"));
	}

	// Populate and set up the detection component
	Detector = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = Detector;
	Detector->SetWorldScale3D(FVector(0.25f, 3.0f, 3.0f));

	// Check if the player reference has been populated
	if (!PlayerRef)
	{
		// Populate the player reference
		PlayerRef = UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass());
	}
}

/**
 * @brief Called when the game starts or when spawned
 */
void AObjectiveParent::BeginPlay()
{
	Super::BeginPlay();

	// Checks if the player reference has been populated
	if (!PlayerRef)
	{
		// Populate the player reference
		PlayerRef = UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass());
	}
}

/**
 * @brief Called every frame
 * @param DeltaTime Amount of time that occurred between frames
 */
void AObjectiveParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * @brief
 * Allows for external population of the spline ref but only if it has not already been provided.
 * @param NewRef The spline to be referenced by the objectives 
 */
void AObjectiveParent::PopulateSplineRef(USplineComponent* NewRef)
{
	// Check if the spline reference has been populated
	if (!SplineRef)
	{
		// Populate the spline reference using the one provided
		SplineRef = NewRef;
	}
	else
	{
		// Log that the spline has already been populated
		UE_LOG(LogTemp, Warning, TEXT("The spline reference has already been populated!"));
	}
}
