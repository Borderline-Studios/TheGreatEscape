// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2023 Media Design School
//
// File Name   :	TrainCarriage.h
// Description :	Contains the declarations for the Train Carriage object. 
// Author      :	Borderline Studios - Jake Laird
// Mail        :	jake.laird@mds.ac.nz
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
};
