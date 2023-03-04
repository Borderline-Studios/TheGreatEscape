// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "TrainTrackParent.generated.h"


class ATrainTrackStart;
class ATrainTrackMiddle;
class ATrainTrackEnd;
UCLASS()
class THEGREATESCAPE_API ATrainTrackParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainTrackParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// VARIABLES

	// FUNCTIONS

	
protected:
	// VARIABLES
	USplineComponent* Spline;
	
	// Static variables available for all of the tracks to access
	// Might be risky letting each of the children have access to these but it is unlikely for
	// each of the separate components to touch anything other than is necessary.
	static ATrainTrackStart* StartRef;

	// FUNCTIONS

	
public:
	// VARIABLES


	// FUNCTIONS

	

	
};
