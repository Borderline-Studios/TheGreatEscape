// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Train/Tracks/TrainTrackParent.h"
#include "TrainTrackStart.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API ATrainTrackStart : public ATrainTrackParent
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATrainTrackStart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UStaticMeshComponent* StartMarker;

	UPROPERTY(VisibleInstanceOnly)
	/*static*/ TArray<ATrainTrackMiddle*> MiddleRefs;

	bool MarkedForDeath = false;

	// FUNCTIONS
	// Standard Functions
	void BeginDestroy() override;
	
	// Editor Functions
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif
protected:


public:
	void AddToArray(ATrainTrackMiddle* NewTrackRef);
	void RemoveFromArray(ATrainTrackMiddle* NewTrackRef);
	
};
