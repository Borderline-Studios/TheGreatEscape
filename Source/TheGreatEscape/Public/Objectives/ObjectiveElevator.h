// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveElevator.h
// Description : Contains the definitions and declarations for the ObjectiveElevator class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ObjectiveElevator.generated.h"

UCLASS()
class THEGREATESCAPE_API AObjectiveElevator : public AActor
{
	GENERATED_BODY()
    
public:    
	// Sets default values for this actor's properties
	AObjectiveElevator();

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
	// Timeline Functions
	UFUNCTION()
	void ProcessMovement(float TimelineProgress);
	UFUNCTION()
	void OnEndMovementTimeline();
	
	void DetermineDirection();

	// VARIABLES
	// Editable Values per instance for testing or change by others
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	int BatteriesRequiredForActivation = 1;
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	int TimeToComplete = 3;
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	float TimeBetweenElevations = 3.0f;

	// Timeline Setup
	FTimeline MovementTimeline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "Editable")
	UCurveFloat* MovementCurve;

	// Timer handle for 
	FTimerHandle TimelineEndHandle;

	// Movement Variables
	FVector StartPos;
	FVector EndPos;
	bool bElevatingUp = true;

	// Bool for tracking whether the elevator has started or not
	bool bElevatorStarted = false;
    
public:
	// FUNCTIONS
	void EnableElevator(int SlotsFilled);
};