// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveSecure.h
// Description : Contains the declarations and definitions for the ObjectiveSecure c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Objectives/ObjectiveParent.h"
#include "ObjectiveSecure.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AObjectiveSecure : public AObjectiveParent
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
	UFUNCTION()
	void EndAreaOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// VARIABLES
	UPROPERTY(EditInstanceOnly)
	UBoxComponent* AreaToSecure;

	UPROPERTY(EditInstanceOnly)	// Change SecondsRequired to a 60 second default once testing is over
	int SecondsRequired = 5;
	int TimeSinceEntered = 0;

	bool bPlayerInZone = false;

public:
	// FUNCTIONS
	AObjectiveSecure();
	virtual void Tick(float DeltaSeconds) override;

	// VARIABLES
};
