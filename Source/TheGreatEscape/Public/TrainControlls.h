// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : TrainControls.h
// Description : Updates orientation of train controls based on the train setting
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainControlls.generated.h"

UENUM(BlueprintType)
enum class ETrainControlSetting : uint8
{
	Slow,
	Normal,
	Fast
};

class ATrainEngine;
UCLASS()
class THEGREATESCAPE_API ATrainControlls : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainControlls();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to update the rotation of the train controls
	void UpdateControls();

	ETrainControlSetting ControlSetting = ETrainControlSetting::Slow;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TrainControlMesh;


private:
	FRotator CurrentRotation;

	float RotateValue;

	ATrainEngine* EngineRef;
};
