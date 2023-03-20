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

/*
 * 
 */
UCLASS()
class THEGREATESCAPE_API AObjectiveGate : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveGate();
	virtual void Destroyed() override;
	
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
	
	UFUNCTION(CallInEditor, Category = "Functionality")
	void SnapRotation() const;

	UFUNCTION(CallInEditor, Category = "Functionality")
	void SpawnPickup();

	UFUNCTION(CallInEditor, Category = "Functionality")
	void RemovePickup();

	UFUNCTION(CallInEditor, Category = "Functionality")
	void ClearPickups();

	bool CleanPickupsArray();
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
	UFUNCTION()
	void EndSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// VARIABLES
	USceneComponent* Root;
	
	UStaticMeshComponent* GateMesh;

	UPROPERTY(EditInstanceOnly, Category = "Functionality")
	ASplineTrack* SplineRef;

	UPROPERTY(EditInstanceOnly, Category = "Functionality")
	bool bSnapToTrack = true;

	USphereComponent* TrainDetector;

	ATrainEngine* EngineRef;

	int DistanceAlongSpline = 0;
	bool bTrainStopped = false;

	UClass* PickupItemClassRef;
	UPROPERTY(VisibleInstanceOnly, Category = "Functionality")
	TArray<AActor*> PickupItems;
	int PickupItemPlacedCount = 0;
	int PickupItemsNum = 0;
	
protected:
	// FUNCTIONS


	// VARIABLES

public:
	// FUNCTIONS


	// VARIABLES

};
