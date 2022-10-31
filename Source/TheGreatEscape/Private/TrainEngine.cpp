// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainEngine.h"
#include "SplineTrack.h"

// Sets default values
ATrainEngine::ATrainEngine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrainEngine::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialising variables
	LerpTimer = 0;
	TimeSinceStart = 0.0f;
	bHasStartedMoving = false;
	SplineLength = -5;
	CompleteSplineLength = 0;

	const float DistancePercentage = DistanceBetweenCars / 100;

	// Checking for a spline reference and connecting if one is detected
	if (!TrackActorRef) {}
	else if (USplineComponent* TempSplineRef = Cast<USplineComponent>(TrackActorRef->GetRootComponent()); TempSplineRef != nullptr)
	{
		TrackSplineRef = TempSplineRef;
	}

	if (TrackSplineRef)
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, TEXT("Train Spline Ref populated"));
		SplineLength = TrackSplineRef->GetSplineLength();

		CarriageOffset = SplineLength * DistancePercentage;
	}

	GEngine->AddOnScreenDebugMessage(2, 5, FColor::Blue, FString::Printf(TEXT("Train Has Started Counting, will begin moving in %d Seconds"), StartDelayTime));

	// Setting up the engine along this spline
	float StartPos = CarriageCount * DistancePercentage;
	EngineStart = FMath::Lerp(0.0f, SplineLength, StartPos);	// The BP has an offset that is negative but flip its sign because that BP assumes a negative traversal of the spline

	SetActorTransform(TrackSplineRef->GetTransformAtDistanceAlongSpline(EngineStart, ESplineCoordinateSpace::World));

	if (CarriageCount > 0)
	{
		for (int i = 0; i < CarriageCount; ++i)
		{
			float temp = (CarriageCount - 1 - i) * DistancePercentage;
			float CarriageStartDist = FMath::Lerp(0.0f, SplineLength, temp);

			FTransform CarriageSpawnTransform = TrackSplineRef->GetTransformAtDistanceAlongSpline(CarriageStartDist, ESplineCoordinateSpace::World);
			CarriageSpawnTransform.SetScale3D(FVector(1.0f));

			CarriageRefs.Add(GetWorld()->SpawnActor<ATrainCarriage>(ATrainCarriage::StaticClass(), CarriageSpawnTransform));
		}
	}
}

// Called every frame
void ATrainEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Initial, effectively a do-once but in Tick so that it's after the begin play
	// Populating the Train Track References
	if (ASplineTrack* TempTrackRef = Cast<ASplineTrack>(TrackActorRef); TempTrackRef && CompleteTrackRefs.Num() == 0)
	{
		TempTrackRef->PopulateTrainRef(this);
		//CompleteTrackRefs.AddUnique(TempTrackRef);

		while (TempTrackRef->GetNextSpline())
		{
			TempTrackRef->GetSpline()->SetUnselectedSplineSegmentColor(FColor::Cyan);
		
			CompleteTrackRefs.AddUnique(TempTrackRef);
			CompleteSplineLength += TempTrackRef->GetSpline()->GetSplineLength();
			TempTrackRef = TempTrackRef->GetNextSpline();
			TempTrackRef->PopulateTrainRef(this);

			// If this is the final spline, connect that up too before exiting the loop
			if (!TempTrackRef->GetNextSpline())
			{
				CompleteTrackRefs.AddUnique(TempTrackRef);
				CompleteSplineLength += TempTrackRef->GetSpline()->GetSplineLength();
				TempTrackRef->GetSpline()->SetUnselectedSplineSegmentColor(FColor::Cyan);
			}
		}

		for (int i = 0; i < CompleteTrackRefs.Num(); i++)
		{
			FSplineTraversalParameters NewSplineParams;
			NewSplineParams.Ratio = CompleteSplineLength / TimeToComplete;	// There should be a way to make this static but I haven't worked that out yet
			NewSplineParams.LengthToTraverse = CompleteTrackRefs[i]->GetSpline()->GetSplineLength();
			NewSplineParams.TimeToTraverse = NewSplineParams.LengthToTraverse / NewSplineParams.Ratio;
			NewSplineParams.TimeToSwap = 1 / NewSplineParams.TimeToTraverse;
			
			if (SplineTravelParameters.Num() >= 1)
			{
				NewSplineParams.TimeToSwap += SplineTravelParameters[i - 1].TimeToSwap;
			}

			SplineTravelParameters.Add(NewSplineParams);
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Check Success"));
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Magenta, FString::Printf(TEXT("Number of connected splines = %d"), CompleteTrackRefs.Num()));
	}

	// Standard Tick Operation
	
	TimeSinceStart += DeltaTime;
    	
    if (TimeSinceStart > StartDelayTime && bHasStartedMoving == false && TrackSplineRef)
    {
        TimeSinceStart -= StartDelayTime;
        bHasStartedMoving = true;
        
        GEngine->AddOnScreenDebugMessage(2, 5, FColor::Green, TEXT("Train has started moving"));
    }

    if (bHasStartedMoving)
    {
	    GEngine->AddOnScreenDebugMessage(3, DeltaTime, FColor::Magenta, FString::Printf(TEXT("LerpTimer = %f"), (TimeSinceStart/TimeToComplete)));
    	GEngine->AddOnScreenDebugMessage(4, DeltaTime, FColor::Magenta, FString::Printf(TEXT("Time To Complete (In Seconds) = %d"), TimeToComplete));

	    // for (int i = 0; i < CompleteTrackRefs.Num(); ++i)
	    // {
		   //  USplineComponent* temp = CompleteTrackRefs[i]->GetSpline();
	    //
		   //  if (temp == TrackSplineRef && SplineTravelParameters[i].TimeToTraverse != CurrentSplineTimeToTraverse)
		   //  {
			  //   CurrentSplineTimeToTraverse = SplineTravelParameters[i].TimeToTraverse;
		   //  	break;
		   //  }
	    // }

    	// GEngine->AddOnScreenDebugMessage(25, DeltaTime, FColor::Magenta, FString::Printf(TEXT("CurrentSplineTimeToTraverse = %f"), CurrentSplineTimeToTraverse));

    	float TimerTrack = TimeSinceStart / TimeToComplete;

    	// GEngine->AddOnScreenDebugMessage(26, DeltaTime, FColor::Magenta, FString::Printf(TEXT("TimerTrack = %f"), TimerTrack));

	    if (TimerTrack < 0)
	    {
	    	// TimerTrack = 0;
	    	TimeSinceStart = 0; 
	    }
    	else if (TimerTrack >= 1)
	    {
		    // TimerTrack -= 1;
    		TimeSinceStart = 0;
	    }

	    const float CurrentDistance = FMath::Lerp(EngineStart, SplineLength, TimerTrack);

    	SetActorLocation(TrackSplineRef->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));
    	SetActorRotation(TrackSplineRef->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));

    	//SetActorTransform(TrackSplineRef->GetTransformAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));

	    if (CarriageCount > 0)
	    {
	        for (int i = 0; i < CarriageCount; ++i)
	        {
		        const float CurrentCarriageOffset = (i + 1) * CarriageOffset;
	        	const float CurrentCarriageDistance = CurrentDistance - CurrentCarriageOffset;

	        	CarriageRefs[i]->SetActorLocation(TrackSplineRef->GetLocationAtDistanceAlongSpline(CurrentCarriageDistance, ESplineCoordinateSpace::World));
	        	CarriageRefs[i]->SetActorRotation(TrackSplineRef->GetRotationAtDistanceAlongSpline(CurrentCarriageDistance, ESplineCoordinateSpace::World));
	        	//CarriageRefs[i]->SetActorTransform(TrackSplineRef->GetTransformAtDistanceAlongSpline(CurrentCarriageDistance, ESplineCoordinateSpace::World));
	        }
	    }
    }
}

bool ATrainEngine::ChangeTrack(AActor* NewTrack)
{
	if (USplineComponent* TempSplineRef = Cast<USplineComponent>(NewTrack->GetRootComponent()); TempSplineRef == nullptr || ShouldChangeTracks == false)
	{
		return false;
	}
	else
	{
		TrackSplineRef = TempSplineRef;
	}

	TimeSinceStart = 0;
	//SplineLength = TrackSplineRef->GetSplineLength();

	return true;
}

void ATrainEngine::GetSplineReferences(TArray<ASplineTrack*>& Array)
{
	Array = CompleteTrackRefs;
}

void ATrainEngine::BPOverrideTrack(AActor* TrackOverride)
{
	if (USplineComponent* TempSplineRef = Cast<USplineComponent>(TrackOverride->GetRootComponent()); TempSplineRef != nullptr)
	{
		TrackSplineRef = TempSplineRef;
	}

	if (TrackSplineRef)
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("Train Spline Ref Overridden"));
		SplineLength = TrackSplineRef->GetSplineLength();
	}
	
	EngineStart = FMath::Lerp(0.0f, SplineLength, 0.0f);
	SetActorTransform(TrackSplineRef->GetTransformAtSplinePoint(0, ESplineCoordinateSpace::World));
	isTrackOverridden = true;
}

bool ATrainEngine::GetTrackOverrideState()
{
	return isTrackOverridden;
}
