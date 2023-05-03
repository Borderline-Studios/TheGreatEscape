// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveGateBatterySlot.generated.h"

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

	// reference to player
	inline static APlayerCharacter* PlayerRef;

	// reference to gate
	AObjectiveGate* GateRef;

	// Pickup Item Class
	UClass* PickupItemClassRef;

	// Reference to item to pick up
	AActor* PickupItem;

	// Detection Sphere
	USphereComponent* DetectionSphere;

	// Static Mesh Component
	UStaticMeshComponent* SlotMesh;
	
protected:
	// FUNCTIONS


	// VARIABLES


public:
	// FUNCTIONS
	bool GetSlotFilled();
	void SpawnPickup(UClass* NewPickupItemClass);
	void InitialiseFromGate(AObjectiveGate* NewGateRef, UClass* NewPickupItemClass);

	// VARIABLES

};
