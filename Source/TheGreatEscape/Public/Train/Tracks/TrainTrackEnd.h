// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Train/Tracks/TrainTrackParent.h"
#include "TrainTrackEnd.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API ATrainTrackEnd : public ATrainTrackParent
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATrainTrackEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
