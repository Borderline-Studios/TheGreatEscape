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
#include "ObjectiveGateBatterySlot.generated.h"

// Forward declared classes
class USphereComponent;
class AObjectiveGate;
class APlayerCharacter;

UCLASS()
class THEGREATESCAPE_API AObjectiveGateBatterySlot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveGateBatterySlot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when the game ends or lifetime of the instance is coming to an end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// FUNCTIONS
	UFUNCTION()
	void BeginBatteryDetectorOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);
	
	virtual void Destroyed() override;
	
	// VARIABLES
	bool bSlotFilled = false;

	// Reference to player
	inline static APlayerCharacter* PlayerRef;

	// Reference to gate
	AObjectiveGate* GateRef;

	// Pickup Item Class
	UClass* PickupItemClassRef;

	// Reference to item to pick up
	AActor* PickupItem;

	// Detection Sphere
	USphereComponent* DetectionSphere;

	// Static Mesh Component
	UStaticMeshComponent* SlotMesh;
	
public:
	// FUNCTIONS
	bool GetSlotFilled();
	void SpawnPickup(UClass* NewPickupItemClass);
	void InitialiseFromGate(AObjectiveGate* NewGateRef, UClass* NewPickupItemClass);

	// VARIABLES

};
