// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "HandCart.generated.h"

UCLASS()
class THEGREATESCAPE_API AHandCart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandCart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Custom section
public:
	UPROPERTY(EditInstanceOnly)
	AActor* TrackActorRef;
	
protected:


private:
	// Spline Reference
	USplineComponent* SplineRef;
	// Player Reference
	AActor* PlayerRef;

	// Spline Tracking
	float TimeSinceStart = 0;

	UPROPERTY(EditInstanceOnly)
	float TempTTC = 10;

	float EngineStart = 0;

	// Mesh Variables
	USceneComponent* SceneRoot;
	UStaticMeshComponent* Base;
	UStaticMeshComponent* Middle;
	UPROPERTY(EditInstanceOnly)
	UStaticMeshComponent* Handle;
	UStaticMeshComponent* FGrip;
	UStaticMeshComponent* BGrip;
	UArrowComponent* Arrow;

	// Collision Components
	UBoxComponent* FrontCollision;
	UBoxComponent* BackCollision;

	// Collision Overlaps
	bool IsFrontCollided = false;
	bool IsBackCollided = false;
	// Front
	UFUNCTION()
	void BeginFrontOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);
	UFUNCTION()
	void EndFrontOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// Back
	UFUNCTION()
	void BeginBackOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);
	UFUNCTION()
	void EndBackOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// Initialised
	bool bIsInitialised = false;
};
