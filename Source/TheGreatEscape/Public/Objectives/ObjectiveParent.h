// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveParent.h
// Description : Contains the declarations and definitions for the ObjectiveParent c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveParent.generated.h"

// Forward declared classes
class UBoxComponent;
class USplineComponent;

UCLASS()
class THEGREATESCAPE_API AObjectiveParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// FUNCTIONS
	static void PopulateSplineRef(USplineComponent* NewRef);

	/**
	 * @brief Checks to see if the spline reference has been populated or not
	 * @return Whether or not the spline reference has been populated
	 */
	static bool SplineRefPopulated()
	{
		if (SplineRef)
		{
			return true;
		}
		
		return false;
	}

protected:
	// VARIABLES
	static USplineComponent* SplineRef;
	UBoxComponent* Detector;
	AActor* PlayerRef;
	bool bPlayerSucceeded = false;
};