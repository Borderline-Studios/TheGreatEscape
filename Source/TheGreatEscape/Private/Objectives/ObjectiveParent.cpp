// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   :
// Description :
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        : 

#include "Objectives/ObjectiveParent.h"

#include "Components/BoxComponent.h"

// Static Variable Declarations
USplineComponent* AObjectiveParent::SplineReference;

// Sets default values
AObjectiveParent::AObjectiveParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (!SplineReference)
    {
	    UE_LOG(LogTemp, Warning, TEXT("Missing a Spline Reference"));
    }

	Detector = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = Detector;
	
}

// Called when the game starts or when spawned
void AObjectiveParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
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
	if (!SplineReference)
	{
		SplineReference = NewRef;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The spline reference has already been populated!"));
	}
}

