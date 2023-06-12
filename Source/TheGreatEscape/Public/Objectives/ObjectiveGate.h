// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveGate.h
// Description : Contains the definitions and declarations for the ObjectiveGate class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveGate.generated.h"

class UGateSphereComponent;
// Forward Declared classes
class AObjectiveElevator;
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

private:
	// FUNCTIONS
// This means that these functions only exist and apply while in the editor.
// These do not apply to packages
#if WITH_EDITOR
	// Unreal standard functions
	virtual void Destroyed() override;

  	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// Custom functions
	// Allow for editor functionality when setting up the gate
	UFUNCTION(CallInEditor, Category = "Functionality")
	void SnapRotation() const;

	UFUNCTION(CallInEditor, Category = "Functionality")
	void SpawnPickup();

	UFUNCTION(CallInEditor, Category = "Functionality")
	void RemovePickup();

	UFUNCTION(CallInEditor, Category = "Functionality")
	void ClearPickups();
#endif

	// Fixing Broken References
	UFUNCTION(CallInEditor, Category = "Functionality")
	void FixReferences();
	bool CleanSlotsArray();
	
	UFUNCTION()
	void BeginTrainDetectorOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);

	int GetSlotsFilled();

	// VARIABLES
	// Object Root
	USceneComponent* Root;

	// Player reference
    APlayerCharacter* PlayerRef;

	// Static mesh components
	UStaticMeshComponent* GateFrame;
	UStaticMeshComponent* GateLeft;
	UStaticMeshComponent* GateRight;
	
	// Movement setup variables
	float TimeSinceEnabled = 0;
	const int DoorMoveDistance = 650;
	bool bOpened = false;

	// Gate door positions
	FVector LeftGateRelativeLocation;
	FVector RightGateRelativeLocation;

	// Spline reference
	UPROPERTY(EditInstanceOnly, Category = "Functionality")
	ASplineTrack* SplineRef;

	// Editor variable for gate positioning
	UPROPERTY(EditInstanceOnly, Category = Functionality)
	bool bSnapToTrack = true;

	// Gate sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX",  meta = (AllowPrivateAccess = "true"))
	USoundBase* GateSFX;

	// Train Detection Component
	UPROPERTY(VisibleInstanceOnly)
	UGateSphereComponent* TrainDetector;

	// Reference to the train engine
	ATrainEngine* EngineRef;

	// Which train platforms do we want to open?
	UPROPERTY(EditInstanceOnly, Category = "Functionality")
	bool bOpenLeftPlatform = false;
	UPROPERTY(EditInstanceOnly, Category = "Functionality")
	bool bOpenRightPlatform = false;

	// Elevator reference. Populated in editor only if a relevant elevator is required.
	UPROPERTY(EditInstanceOnly, Category = "Functionality", meta = (AllowPrivateAccess = "true"))
	AObjectiveElevator* ElevatorRef;

	// Array of references to battery slots
	UPROPERTY(VisibleInstanceOnly)
	TArray<AActor*> SlotRefs;

	// Bool for whether the train has been stopped
	bool bTrainStopped = false;

	// Class references for spawning actors
	UClass* SlotClassRef;
	UClass* PickupItemClassRef;

public:
	// FUNCTIONS
	UFUNCTION(BlueprintCallable)
	int ElevatorInformationCheck(int RequirementToAdjust);

	void RepopulateBatteryObjectiveText() const;
	void UpdateFromSlot();
	
	void UpdateObjectiveText(FString NewText = "") const;
};