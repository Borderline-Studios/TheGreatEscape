// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainEngine.h"
#include "SplineTrack.h"
#include "Kismet/KismetMathLibrary.h"

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
	CurrentSplineIndex = 0;

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
			CarriageRefs[i]->TrackSplineRef = TrackSplineRef;
		}
	}
}

// Called every frame
void ATrainEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Initial, effectively a do-once but in Tick so that it's after the begin play
	// Populating the Train Track References
	if (ASplineTrack* TempTrackRef = Cast<ASplineTrack>(TrackActorRef); TempTrackRef && CompleteTrackRefs.IsEmpty())
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
				TempTrackRef->GetSpline()->SetUnselectedSplineSegmentColor(FColor::Cyan);
				CompleteTrackRefs.AddUnique(TempTrackRef);
				CompleteSplineLength += TempTrackRef->GetSpline()->GetSplineLength();
			}
		}

		GEngine->AddOnScreenDebugMessage(20, 20, FColor::Emerald, FString::Printf(TEXT("Complete Spline Length = %f"), CompleteSplineLength));

		for (int i = 0; i < CompleteTrackRefs.Num(); i++)
		{
			FSplineTraversalParameters NewSplineParams;
			NewSplineParams.Ratio = CompleteSplineLength / TimeToComplete;	// There should be a way to make this static but I haven't worked that out yet
			NewSplineParams.LengthToTraverse = CompleteTrackRefs[i]->GetSpline()->GetSplineLength();
			
			// GEngine->AddOnScreenDebugMessage((i + 1) * 100, 20, FColor::Magenta, FString::Printf(TEXT("Length for Spline %d: %f"), i + 1, NewSplineParams.LengthToTraverse));
			// GEngine->AddOnScreenDebugMessage((i + 1) * 101, 20, FColor::Magenta, FString::Printf(TEXT("Complete Spline Length: %f"), CompleteSplineLength));

			NewSplineParams.TimeToTraverse = NewSplineParams.LengthToTraverse / CompleteSplineLength /*/ NewSplineParams.Ratio*/;
			NewSplineParams.TimeToSwap = /*1 / */NewSplineParams.TimeToTraverse;
			
			if (SplineTravelParameters.Num() >= 1)
			{
				NewSplineParams.TimeToSwap += SplineTravelParameters[i - 1].TimeToSwap;
			}
			GEngine->AddOnScreenDebugMessage((i + 1) * 102, 20, FColor::Magenta, FString::Printf(TEXT("TimeToSwap for Component %d: %f"), i, NewSplineParams.TimeToSwap));
			// GEngine->AddOnScreenDebugMessage((i + 1) * 103, 20, FColor::Magenta, FString::Printf(TEXT("TimeToTraverse for Component %d: %f"), i, NewSplineParams.TimeToTraverse));

			SplineTravelParameters.Add(NewSplineParams);

			// GEngine->AddOnScreenDebugMessage(21 + i, 20, FColor::Emerald, FString::Printf(TEXT("Spline #%d Length: %f"), i + 1, CompleteTrackRefs[i]->GetSpline()->GetSplineLength()));
		}

		// UKismetMathLibrary::InRange_FloatFloat();
		
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
	    const float TimerTrack = TimeSinceStart / TimeToComplete;

	    if (CurrentSplineIndex >= 0 && CurrentSplineIndex < CompleteTrackRefs.Num())
	    {
	    	// This segment calculates which spline the engine should be on based on:
	    	// - The location of the engine throughout the complete spline
	    	// - The time since the start of traversing the complete spline.
	    	int NewSplineIndex = 0;
		    
		    for (int i = CompleteTrackRefs.Num() - 1; i > 0; i--)
		    {
		        if (UKismetMathLibrary::InRange_FloatFloat(TimerTrack, SplineTravelParameters[i - 1].TimeToSwap, SplineTravelParameters[i].TimeToSwap, true, false))
		        {
					NewSplineIndex = i;
		        	break;
		        }
		    }

	    	// Updates what track the train is currently on depending on whether or not there is a difference between the current assigned index and the index calculated above.
	    	if (CurrentSplineIndex != NewSplineIndex)
	    	{
	    		GEngine->AddOnScreenDebugMessage(7, 1, FColor::Red, TEXT("Changing Tracks"));
	    		CurrentSplineIndex = NewSplineIndex;
	    		ChangeTrack(CompleteTrackRefs[CurrentSplineIndex]);
	    	}

	    	// This segment handles the conversion of time between the complete spline and the individual spline segments.
	    	// For example, 0.5 seconds through the complete spline might refer to 0.86 seconds through the second spline.
	    	float TimeOffset = 0;

	    	for (int i = 0; i < CurrentSplineIndex; i++)
	    	{
	    		TimeOffset += SplineTravelParameters[i].TimeToTraverse;
	    	}

	    	const float CurrentSplineProgress = FMath::Lerp(EngineStart, CompleteSplineLength, TimerTrack - TimeOffset);

	    	SetActorLocation(TrackSplineRef->GetLocationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World));
	    	SetActorRotation(TrackSplineRef->GetRotationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World));
	    }

    	GEngine->AddOnScreenDebugMessage(3, DeltaTime, FColor::Magenta, FString::Printf(TEXT("Distance Along Spline (Normalised) = %f"), TimerTrack));
    	GEngine->AddOnScreenDebugMessage(4, 10, FColor::Magenta, FString::Printf(TEXT("Time To Complete (In Seconds) = %d"), TimeToComplete));

	    if (!UKismetMathLibrary::InRange_FloatFloat(TimerTrack, 0.0, 1.0, false, true))
	    {
		    TimeSinceStart = 0;
	    }

    	// This code seems expensive. If we can move it to the carriages then it might be more effective.
    	// As it stands, it runs and keeps the train moving through the spline.
    	// However, it causes each train segment to hang on the point of track change.
    	// Might be an issue with resetting the train on the new spline but it seems weird that it happens with each carriage.
    	if (CarriageCount > 0)	
	    {
		    for (int i = 0; i < CarriageRefs.Num(); i++)
		    {
		    	int NewSplineIndex = 0;
		    	const float CarriageTimerTrack = TimerTrack - ((i + 1) * (CompleteSplineLength * DistanceBetweenCars / 100) / CompleteSplineLength);
		    
		    	for (int j = CompleteTrackRefs.Num() - 1; j > 0; j--)
		    	{
		    		if (UKismetMathLibrary::InRange_FloatFloat(CarriageTimerTrack, SplineTravelParameters[j - 1].TimeToSwap, SplineTravelParameters[j].TimeToSwap, true, false))
		    		{
		    			NewSplineIndex = j;
		    			break;
		    		}
		    	}

		    	if (CarriageRefs[i]->CurrentSplineIndex != NewSplineIndex)
		    	{
		    		GEngine->AddOnScreenDebugMessage(5000 + i, 1, FColor::Red, FString::Printf(TEXT("Carriage %d changes track"), i + 1));
		    		CarriageRefs[i]->CurrentSplineIndex = NewSplineIndex;
		    		CarriageRefs[i]->ChangeTrack(CompleteTrackRefs[CarriageRefs[i]->CurrentSplineIndex]);
		    	}

		    	float TimeOffset = 0;

		    	for (int j = 0; j < CarriageRefs[i]->CurrentSplineIndex; j++)
		    	{
		    		TimeOffset += SplineTravelParameters[j].TimeToTraverse;
		    	}

		    	const float CarriageSplineProgress = FMath::Lerp(EngineStart, CompleteSplineLength, CarriageTimerTrack - TimeOffset);

		    	CarriageRefs[i]->SetActorLocation(CarriageRefs[i]->TrackSplineRef->GetLocationAtDistanceAlongSpline(CarriageSplineProgress, ESplineCoordinateSpace::World));
		    	CarriageRefs[i]->SetActorRotation(CarriageRefs[i]->TrackSplineRef->GetRotationAtDistanceAlongSpline(CarriageSplineProgress, ESplineCoordinateSpace::World));
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

	//TimeSinceStart = 0;
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
