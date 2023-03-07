// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "TrainCarriage.generated.h"

class USplineComponent;
UCLASS()
class THEGREATESCAPE_API ATrainCarriage : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATrainCarriage();

	void InitialiseFromEngine(int CarriageNum, UStaticMesh* AssignedMesh, USplineComponent* NewSplineRef);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// VARIABLES
	
	// FUNCTIONS
	void ProcessMovement(float EngineSplineDist);
	
private:
	// VARIABLES
	int CarriageNumber;
	
	UPROPERTY(EditInstanceOnly)
	USceneComponent* SceneRoot;

	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* Box;

	UPROPERTY(EditInstanceOnly)
	UArrowComponent* Arrow;

	// inline static TStaticArray<int, 4> CarriageDistances = TStaticArray<int, 4>(EInPlace::InPlace, 1800);
	float DistanceFromFront = 0;

	USplineComponent* SplineRef;

	// FUNCTIONS
	// // Sets default values for this actor's properties
	// ATrainCarriage();
};
