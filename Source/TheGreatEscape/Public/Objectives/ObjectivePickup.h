// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectivePickup.h
// Description : Contains the declarations and definitions for the ObjectivePickup c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Objectives/ObjectiveParent.h"
#include "ObjectivePickup.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AObjectivePickup : public AObjectiveParent
{
	GENERATED_BODY()

private:
	// FUNCTIONS
	UFUNCTION()
	void BeginAreaOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);

	// VARIABLES
	// Drop-off zone
	UPROPERTY(EditInstanceOnly)
	UBoxComponent* CollectionZone;
	// Pickup Item
	UPROPERTY(EditInstanceOnly)
	AActor* ObjectToPickup;

public:
	// FUNCTIONS
	AObjectivePickup();
	virtual void Tick(float DeltaSeconds) override;

	// VARIABLES
};
