// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : SplineTrack.h
// Description : Contains the declarations and definitions for the SplineTrack c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "SplineTrack.generated.h"

// Forward declared class
class ATrainEngine;

UCLASS()
class THEGREATESCAPE_API ASplineTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// FUNCTIONS
	// Train Reference Population
	void PopulateTrainRef(ATrainEngine* NewTrainRef);
	// Spline Functions
	USplineComponent* GetSpline() const;
	ASplineTrack* GetNextSpline();
	
	// VARIABLES
	
private:
	// VARIABLES
	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	// Detection Components
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Start;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Final;

	// Train reference
	ATrainEngine* TrainRef;
	// Reference to Next Spline
	ASplineTrack* NextSpline;

	// FUNCTIONS
	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSplineComponent() const;
	UBoxComponent* GetStartBoxCollider() const;

#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
#endif
};