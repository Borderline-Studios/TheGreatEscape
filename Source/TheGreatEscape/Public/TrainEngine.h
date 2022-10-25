// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "TrainEngine.generated.h"

UCLASS()
class THEGREATESCAPE_API ATrainEngine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainEngine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Custom section containing everything added beyond the default Unreal code
	public:
	// Track Changing
	UPROPERTY(BlueprintReadWrite)
	bool ShouldChangeTracks = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)		// The actor passed in from the editor
	AActor* TrackActorRef;

	// Functions
	UFUNCTION(BlueprintCallable)
	bool ChangeTrack(AActor* NewTrack);

protected:


private:
	// Variables
	// Obtaining the spline for the train to follow
	// UPROPERTY(EditInstanceOnly, BlueprintReadWrite)		// The actor passed in from the editor
	// AActor* TrackActorRef;
	USplineComponent* TrackSplineRef;
	float SplineLength;
	UPROPERTY(EditInstanceOnly)
	int TimeToComplete = 30;
	float TimeSinceStart;
	bool bHasStartedMoving;

	// Creating the timer handle used to start the movement after a short delay
	FTimerHandle StartMoveTimerHandle;

	// Used to keep track for the lerp
	float LerpTimer;

	// Track Changing
	// UPROPERTY(BlueprintReadWrite)
	// bool ShouldChangeTracks = false;

	// Functions
};
