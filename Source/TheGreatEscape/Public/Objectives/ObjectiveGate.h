// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   :
// Description :
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveGate.generated.h"

class USphereComponent;
class ATrainEngine;
class ASplineTrack;
class USplineComponent;
UCLASS()
class THEGREATESCAPE_API AObjectiveGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// FUNCTIONS
#if WITH_EDITOR
  	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
	
	UFUNCTION(CallInEditor, Category = "Gate Functions")
	void SnapRotation() const;
#endif

	UFUNCTION()
	void BeginSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);

	// VARIABLES
	UStaticMeshComponent* GateMesh;

	UPROPERTY(EditInstanceOnly, Category = "Gate References")
	ASplineTrack* SplineRef;

	UPROPERTY(EditInstanceOnly, Category = "Gate Variables")
	bool bSnapToTrack = true;
	
	USphereComponent* TrainDetector;

	ATrainEngine* EngineRef;

	UPROPERTY(VisibleInstanceOnly)
	UStaticMeshComponent* PickupItem;

	int DistanceAlongSpline = 0;
	bool TrainStopped = false;
	
protected:
	// FUNCTIONS


	// VARIABLES

public:
	// FUNCTIONS


	// VARIABLES

};
