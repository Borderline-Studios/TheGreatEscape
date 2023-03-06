// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Tracks/TrainTrackMiddle.h"

#include "Train/Tracks/TrainTrackStart.h"
#include "Train/Tracks/TrainTrackEnd.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ATrainTrackMiddle::ATrainTrackMiddle()
{
	Spline->SetUnselectedSplineSegmentColor(FLinearColor(FColor::Blue));
	Spline->SetSelectedSplineSegmentColor(FLinearColor(FColor::Orange));
	Spline->SetTangentColor(FLinearColor(FColor::Magenta));

	PopulateStartRef();
}

void ATrainTrackMiddle::BeginPlay()
{
	Super::BeginPlay();
}

void ATrainTrackMiddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrainTrackMiddle::PopulateStartRef()
{
	if (!StartRef)
	{
		if (ATrainTrackStart* TempRef = Cast<ATrainTrackStart>(UGameplayStatics::GetActorOfClass(this, ATrainTrackStart::StaticClass())))
		{
			StartRef = TempRef;
		}
	}
}

#if WITH_EDITOR
void ATrainTrackMiddle::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	PopulateStartRef();

	if (IsSnappingEnabled && StartRef)
	{
		if (true)
		// {
		// 	
		// }
		// else
		{
			const USplineComponent* StartSpline = StartRef->GetSpline();
			const int StartSplineEndIndex = StartSpline->GetNumberOfSplinePoints();
		
			const FVector StartRefEndLocation = StartSpline->GetWorldLocationAtSplinePoint(StartSplineEndIndex);
			const FRotator StartRefEndRotation = StartSpline->GetRotationAtSplinePoint(StartSplineEndIndex, ESplineCoordinateSpace::World);

			if (FVector::Dist(GetActorLocation(), StartRefEndLocation) < 100.0f)
			{
				SetActorLocation(StartRefEndLocation);
				SetActorRotation(StartRefEndRotation);
			
				if (bFinished)
				{
					const FVector SplineSnapArriveTangents = StartSpline->GetArriveTangentAtSplinePoint(StartSplineEndIndex, ESplineCoordinateSpace::World);
					const FVector SplineSnapLeaveTangents = StartSpline->GetLeaveTangentAtSplinePoint(StartSplineEndIndex, ESplineCoordinateSpace::World);
		
					Spline->SetTangentsAtSplinePoint(
						0,
						SplineSnapArriveTangents,
						SplineSnapLeaveTangents,
						ESplineCoordinateSpace::World
					);
		
					StartRef->AddToArray(this);
				}
			}
		}
	}
}

void ATrainTrackMiddle::PostEditUndo()
{
	Super::PostEditUndo();
}
#endif
