// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "Test_TrackConnector.generated.h"

UCLASS()
class THEGREATESCAPE_API ATest_TrackConnector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATest_TrackConnector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:


protected:


private:
	// Test using components and seeing if they populate
	UPROPERTY(VisibleAnywhere)
	USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Start;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* End;
};
