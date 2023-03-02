// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainTrackPoint.generated.h"

class ATrainTrack;
UCLASS()
class THEGREATESCAPE_API ATrainTrackPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainTrackPoint();
	~ATrainTrackPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UStaticMeshComponent* Marker;

	ATrainTrack* TrackRef;

	void AddToStaticArray();

protected:
	static TArray<ATrainTrackPoint*> StaticArray;

public:
	static TArray<ATrainTrackPoint*> GetStaticArray();

#if WITH_EDITOR
void PostActorCreated();
#endif
};