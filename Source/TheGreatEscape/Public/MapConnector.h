// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MapConnector.generated.h"

class ASplineTrack;

UCLASS()
class THEGREATESCAPE_API AMapConnector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapConnector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// FUNCTIONS
	UFUNCTION()
	void BeginFrontOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult
	);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif

	// VARIABLES
	UBoxComponent* CollisionDetection;

	UPROPERTY(EditInstanceOnly)
	ASplineTrack* SplineRef;
	
	UPROPERTY(EditInstanceOnly)
	float BlendTime = 1.0f;

	UPROPERTY(EditInstanceOnly)
	TSoftObjectPtr<UWorld> NextWorldRef;

	AActor* PlayerRef;
	UCameraComponent* PlayerCamRef;
	
	// UPROPERTY(EditInstanceOnly)
	float ZHeight = 100.0f;
};
