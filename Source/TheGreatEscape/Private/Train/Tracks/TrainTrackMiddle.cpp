// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Tracks/TrainTrackMiddle.h"

ATrainTrackMiddle::ATrainTrackMiddle()
{
	Spline->SetUnselectedSplineSegmentColor(FLinearColor(FColor::Blue));
	Spline->SetSelectedSplineSegmentColor(FLinearColor(FColor::Orange));
	Spline->SetTangentColor(FLinearColor(FColor::Magenta));
	
	if (!StartRef)
	{
		
	}
}

void ATrainTrackMiddle::BeginPlay()
{
	Super::BeginPlay();
}

void ATrainTrackMiddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
