// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "TrainTrack.generated.h"

class ATrainTrackPoint;
UCLASS()
class THEGREATESCAPE_API ATrainTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// VARIABLES
	void AddToArray(ATrainTrackPoint* NewPoint);
	void RemoveFromArray(ATrainTrackPoint* PointToRemove);

	// FUNCTIONS

	
protected:
	// VARIABLES


	// FUNCTIONS

	
private:
	// VARIABLES
	// UPROPERTY(VisibleInstanceOnly)
	USplineComponent* Spline;

	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* StartNotifier;
	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* EndNotifier;

	UPROPERTY(EditInstanceOnly)
	TArray<ATrainTrackPoint*> PointRefs;

	
	// FUNCTIONS

#if WITH_EDITOR
	// Overrides for code that runs specifically when changes are made in the editor
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
#endif
};
