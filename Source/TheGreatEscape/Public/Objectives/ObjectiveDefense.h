// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "Objectives/ObjectiveParent.h"
#include "ObjectiveDefense.generated.h"

/**
 * Does this actually need an overlap box?
 */
UCLASS()
class THEGREATESCAPE_API AObjectiveDefense : public AObjectiveParent
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
	UBoxComponent* AreaToDefend;

	UPROPERTY(EditInstanceOnly)	// Change SecondsRequired to a 60 second default once testing is over
	int SecondsRequired = 5;
	int TimeSinceEntered = 0;

	bool bPlayerInZone = false;

protected:
	// FUNCTIONS


	// VARIABLES

	public:
	// FUNCTIONS
	AObjectiveDefense();
	virtual void Tick(float DeltaSeconds) override;

	// VARIABLES
};
