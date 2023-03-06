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

	StartCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Start Detector"));
	StartCollider->SetWorldScale3D(FVector(1.0f));
	StartCollider->SetSphereRadius(100.0f);
	StartCollider->OnComponentBeginOverlap.AddDynamic(this, &ATrainTrackMiddle::OnStartSphereBeginOverlap);
	StartCollider->OnComponentEndOverlap.AddDynamic(this, &ATrainTrackMiddle::OnStartSphereEndOverlap);

	EndCollider = CreateDefaultSubobject<USphereComponent>(TEXT("End Detector"));
	EndCollider->SetWorldScale3D(FVector(1.0f));
	EndCollider->SetSphereRadius(1.0f);
	EndCollider->OnComponentBeginOverlap.AddDynamic(this, &ATrainTrackMiddle::OnEndSphereBeginOverlap);
	EndCollider->OnComponentEndOverlap.AddDynamic(this, &ATrainTrackMiddle::OnEndSphereEndOverlap);

	PopulateStartRef();
}

void ATrainTrackMiddle::BeginPlay()
{
	Super::BeginPlay();

	StartCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ATrainTrackMiddle::OnStartSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// if (IsSnappingEnabled)
	// {
	// 	// Checking to see if the object being collides with is a train track piece
	// 	if (ATrainTrackParent* TempRef = Cast<ATrainTrackParent>(OtherActor))
	// 	{
	// 		// If it is, check to ensure it's not an end piece
	// 		if (!Cast<ATrainTrackEnd>(TempRef))
	// 		{
	// 			SnappingTarget = TempRef;
	// 		}
	// 	}
	// }
}

void ATrainTrackMiddle::OnStartSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (IsSnappingEnabled)
	{
		// Check if the reference is populated before doing anything with it
		if (SnappingTarget)
		{
			// If populated, check if the actor we're ending overlap with is the target
			if (SnappingTarget == Cast<ATrainTrackParent>(OtherActor))
			{
				// if all passes, reset the target reference
				SnappingTarget = nullptr;
			}
		}
	}
}

void ATrainTrackMiddle::OnEndSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// To ensure the other actor is ATrainTrackMiddle
	if (ATrainTrackMiddle* TempRef = Cast<ATrainTrackMiddle>(OtherActor))
	{
		// To ensure the other component was a sphere component
		if (USphereComponent* TempCompRef = Cast<USphereComponent>(OtherComp))
		{
			// To ensure the other component was not the end sphere collider
			if (TempCompRef->GetUnscaledSphereRadius() != EndCollider->GetUnscaledSphereRadius())
			{
				TempRef->SnappingTarget = this;
			}
		}
	}
}

void ATrainTrackMiddle::OnEndSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void ATrainTrackMiddle::BeginDestroy()
{
	Super::BeginDestroy();

	StartRef->RemoveFromArray(this);
}

#if WITH_EDITOR
void ATrainTrackMiddle::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	PopulateStartRef();

	StartCollider->SetWorldLocation(Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
	StartCollider->SetWorldRotation(Spline->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World));
	
	EndCollider->SetWorldLocation(Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
	EndCollider->SetWorldRotation(Spline->GetRotationAtSplinePoint(Spline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));

	if (IsSnappingEnabled)
	{
		if (SnappingTarget)
		{
			const USplineComponent* SnapTargetSpline = SnappingTarget->GetSpline();
			const int SnapSplineEndIndex = SnapTargetSpline->GetNumberOfSplinePoints();
		
			const FVector SnapRefEndLocation = SnapTargetSpline->GetWorldLocationAtSplinePoint(SnapSplineEndIndex);
			const FRotator SnapRefEndRotation = SnapTargetSpline->GetRotationAtSplinePoint(SnapSplineEndIndex, ESplineCoordinateSpace::World);

			SetActorLocation(SnapRefEndLocation);
			SetActorRotation(SnapRefEndRotation);
		}
		else if (StartRef)
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

	StartCollider->SetWorldLocation(Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World));
	StartCollider->SetWorldRotation(Spline->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World));
	
	EndCollider->SetWorldLocation(Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
	EndCollider->SetWorldRotation(Spline->GetRotationAtSplinePoint(Spline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::World));
}
#endif
