// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Tracks/TrainTrackEnd.h"

ATrainTrackEnd::ATrainTrackEnd()
{
	Spline->SetUnselectedSplineSegmentColor(FLinearColor(FColor::Red));
	Spline->SetSelectedSplineSegmentColor(FLinearColor(FColor::Turquoise));
	Spline->SetTangentColor(FLinearColor(FColor::Yellow));
}

void ATrainTrackEnd::BeginPlay()
{
	Super::BeginPlay();
}

void ATrainTrackEnd::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
