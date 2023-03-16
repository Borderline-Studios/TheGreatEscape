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
	UPROPERTY(EditInstanceOnly)
	UBoxComponent* CollectionZone;
	UPROPERTY(EditInstanceOnly)
	AActor* ObjectToPickup;

public:
	// FUNCTIONS
	AObjectivePickup();
	virtual void Tick(float DeltaSeconds) override;

	// VARIABLES
};
