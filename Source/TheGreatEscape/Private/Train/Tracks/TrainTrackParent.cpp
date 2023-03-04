// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Tracks/TrainTrackParent.h"

// Sets default values
ATrainTrackParent::ATrainTrackParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	RootComponent = Spline;
}

// Called when the game starts or when spawned
void ATrainTrackParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrainTrackParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

