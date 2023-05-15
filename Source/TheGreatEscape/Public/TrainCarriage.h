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

#include "Train/TrainCarParent.h"

#include "TrainCarriage.generated.h"

// Forward declared classes
class APlayerCharacter;
class ATrainEngine;
class UBoxComponent;
class USplineComponent;
class ALight;

UCLASS()
class THEGREATESCAPE_API ATrainCarriage : public ATrainCarParent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATrainCarriage();

	void InitialiseFromEngine(int CarriageNum, int InitDistanceFromFront, UStaticMesh* AssignedMesh, USplineComponent* NewSplineRef, ATrainEngine* NewEngineRef);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called after components are initialised
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// VARIABLES

	
	// FUNCTIONS
	void ProcessMovement(float EngineSplineDist);
	UBoxComponent* GetPlayerDetectionComponent() const;
	
private:
	// FUNCTIONS
	virtual void BeginCarOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		) override;
	virtual void EndCarOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
		) override;
	
	// VARIABLES
	int CarriageNumber;
	float DistanceFromFront = 0;	
	TStaticArray<ALight*, 2> LightRefs;
	static ATrainEngine* EngineRef;
	
};
