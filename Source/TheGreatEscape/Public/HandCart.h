// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	USplineComponent* SplineRef;
	
protected:


private:

};
