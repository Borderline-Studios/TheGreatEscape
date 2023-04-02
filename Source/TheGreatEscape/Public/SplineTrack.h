// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2023 Media Design School
//
// File Name   :	SplineTrack.h
// Description :	Contains the declarations for the SplineTrack object
// Author      :	Borderline Studios - Jake Laird
// Mail        :	jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "TrainEngine.h"
#include "GameFramework/Actor.h"
#include "SplineTrack.generated.h"

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

	// VARIABLES
	
	// FUNCTIONS
	void PopulateTrainRef(ATrainEngine* NewTrainRef);
	USplineComponent* GetSpline() const;
	ASplineTrack* GetNextSpline();

protected:


private:
	// VARIABLES
	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Start;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Final;

	ATrainEngine* TrainRef;
	ASplineTrack* NextSpline;

	// FUNCTIONS
	UFUNCTION(BlueprintCallable)
	USplineComponent* GetSplineComponent() const;
	UBoxComponent* GetStartBoxCollider() const;

#if WITH_EDITOR
	// Overrides for code that runs specifically when changes are made in the editor
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
#endif
};