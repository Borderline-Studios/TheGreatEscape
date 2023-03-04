// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Tracks/TrainTrackStart.h"

ATrainTrackStart::ATrainTrackStart()
{
	Spline->SetUnselectedSplineSegmentColor(FLinearColor(FColor::Green));
	Spline->SetSelectedSplineSegmentColor(FLinearColor(FColor::Purple));
	Spline->SetTangentColor(FLinearColor(FColor::Cyan));
}

void ATrainTrackStart::BeginPlay()
{
	Super::BeginPlay();
}

void ATrainTrackStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
