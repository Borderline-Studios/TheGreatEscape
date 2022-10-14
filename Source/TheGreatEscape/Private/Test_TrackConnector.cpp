// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_TrackConnector.h"

// Sets default values
ATest_TrackConnector::ATest_TrackConnector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Might need a do once gate here to make sure that the spline only populates the first time it's dragged into scene

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Comp"));
	Spline->SetupAttachment(RootComponent);

	Start = CreateDefaultSubobject<UBoxComponent>(TEXT("Start TriggerBox"));
	Start->SetupAttachment(RootComponent);

	End = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	End->SetupAttachment(RootComponent);

	// The issue is that these locations aren't determined until after the construction script runs
	// Perhaps some dummy scene components can be an analogue?
	Start->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	End->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
}

// Called when the game starts or when spawned
void ATest_TrackConnector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATest_TrackConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

