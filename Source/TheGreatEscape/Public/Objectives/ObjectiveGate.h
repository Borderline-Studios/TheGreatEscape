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
class APlayerCharacter;

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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateObjectiveText(FString NewText = "") const;

private:
	// FUNCTIONS
#if WITH_EDITOR
	virtual void Destroyed() override;

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

	UFUNCTION(CallInEditor, Category = "Functionality")
	void FixReferences();

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

    APlayerCharacter* PlayerRef;
	
	UStaticMeshComponent* GateFrame;
	UStaticMeshComponent* GateLeft;
	UStaticMeshComponent* GateRight;
	float TimeSinceEnabled = 0;
	const int DoorMoveDistance = 650;
	bool bOpened = false;

	FVector LeftGateRelativeLocation;
	FVector RightGateRelativeLocation;

	UPROPERTY(EditInstanceOnly, Category = "Functionality")
	ASplineTrack* SplineRef;

	UPROPERTY(EditInstanceOnly, Category = Functionality)
	bool bSnapToTrack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX",  meta = (AllowPrivateAccess = "true"))
	USoundBase* GateSFX;

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
