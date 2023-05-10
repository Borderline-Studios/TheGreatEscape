 // // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// FUNCTIONS
	UFUNCTION()
	void ProcessMovement(float TimelineProgress);
	UFUNCTION()
	void OnEndMovementTimeline();

	void DetermineDirection();

	// VARIABLES
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	int BatteriesRequiredForActivation = 1;
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	int TimeToComplete = 3;
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess="true"))
	float TimeBetweenElevations = 3.0f;

	// Timeline
	FTimeline MovementTimeline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "Editable")
	UCurveFloat* MovementCurve;

	FTimerHandle TimelineEndHandle;
    
	FVector StartPos;
	FVector EndPos;
	bool bElevatingUp = true;

	bool bElevatorStarted = false;
    
public:
	// FUNCTIONS
	void EnableElevator(int SlotsFilled);
};