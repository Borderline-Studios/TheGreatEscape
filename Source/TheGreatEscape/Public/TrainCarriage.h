// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TrainCarriage.generated.h"

UCLASS()
class THEGREATESCAPE_API ATrainCarriage : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATrainCarriage();

	ATrainCarriage(int AssignedNumber);
	
	// // Sets default values for this actor's properties
	// explicit ATrainCarriage(int AssignedNumber);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// VARIABLES
	int CurrentSplineIndex;
	class USplineComponent* TrackSplineRef;
	
	// FUNCTIONS
	void SetCarriageNumber(int AssignedNumber);
	bool ChangeTrack(AActor* NewTrack);
	
private:
	// VARIABLES
	int CarriageNumber;
	
	UPROPERTY(EditInstanceOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* Box;

	UPROPERTY(EditInstanceOnly)
	UArrowComponent* Arrow;

	// FUNCTIONS
	// // Sets default values for this actor's properties
	// ATrainCarriage();
};
