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
#include "TrainEngine.h"
#include "GameFramework/Actor.h"
#include "TrainControlls.generated.h"

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

	// FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void ControlsInteraction();
	// Function to update the rotation of the train controls
	void UpdateHandleRotation(bool bTrainMoving);
	// Allows external classes to play a lever sound
	void PlayLeverSound();

	// VARIABLES
	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* ControlHandle;
	UStaticMeshComponent* ControlBase;

private:
	// FUNCTIONS
	UFUNCTION()
	void ProcessRotation(float TimelineProgress) const;
	
	// VARIABLES
	ATrainEngine* EngineRef;
	FRotator CurrentRotation;

	// Movement
	FTimeline RotationTimeline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCurveFloat* RotationCurve = nullptr;
	const FRotator ForwardRotation = FRotator(0.0f, 0.0f, 80.0f);
	const FRotator BackwardRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*> LeverSFX;
};
