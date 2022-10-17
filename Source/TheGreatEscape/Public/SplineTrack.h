// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "SplineTrack.generated.h"

UCLASS()
class THEGREATESCAPE_API ASplineTrack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineTrack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:


protected:


private:
	// VARIABLES
	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Start;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Final;

	UPROPERTY(EditAnywhere)
	AActor* OtherActor;

	// FUNCTIONS
	USplineComponent* GetSplineComponent() const;
	UBoxComponent* GetStartBoxCollider() const;
	// UFUNCTION()
	// void BeginOverlap(
	// 	UPrimitiveComponent* OverlappedComponent,
	// 	AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp,
	// 	int32 OtherBodyIndex,
	// 	bool bFromSweep,
	// 	const FHitResult &SweepResult
	// );

	// Overrides for code that runs specifically when changes are made in the editor
	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditUndo() override;
};