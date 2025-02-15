// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Train/Tracks/TrainTrackParent.h"
#include "TrainTrackMiddle.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API ATrainTrackMiddle : public ATrainTrackParent
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATrainTrackMiddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// VARIABLES
	UPROPERTY(EditInstanceOnly)
	bool IsSnappingEnabled = true;
	
	// FUNCTIONS
	void PopulateStartRef();

	// void BeginDestroy() override;
	
	// Editor Functions
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
#endif	
protected:

	
public:

	
};
