// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainEngine.h"

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

	if (USplineComponent* TempSplineRef = Cast<USplineComponent>(TrackActorRef->GetRootComponent()); TempSplineRef != nullptr)
	{
		TrackSplineRef = TempSplineRef;
	}

	if (TrackSplineRef != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, TEXT("Train Spline Ref populated"));
		SplineLength = TrackSplineRef->GetSplineLength();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("Train Spline Ref empty"));
	}
	
	GEngine->AddOnScreenDebugMessage(2, 5, FColor::Blue, TEXT("Train Has Started Counting, will begin moving in 10 Seconds"));
}

// Called every frame
void ATrainEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimeSinceStart += DeltaTime;
    	
    	if (TimeSinceStart > 10 && bHasStartedMoving == false && SplineLength != -5)
    	{
    		TimeSinceStart -= 10;
    		bHasStartedMoving = true;
    	    
    		GEngine->AddOnScreenDebugMessage(2, 5, FColor::Green, TEXT("Train has started moving"));
    		SetActorTransform(
    			TrackSplineRef->GetTransformAtDistanceAlongSpline(0, ESplineCoordinateSpace::World, false),
    			false,
    			nullptr,
    			ETeleportType::TeleportPhysics
    			);
    	}
    
    	if (bHasStartedMoving)
    	{
    		GEngine->AddOnScreenDebugMessage(3, DeltaTime, FColor::Magenta, FString::Printf(TEXT("LerpTimer = %f"), (TimeSinceStart/TimeToComplete)));
    		GEngine->AddOnScreenDebugMessage(4, DeltaTime, FColor::Magenta, FString::Printf(TEXT("Time To Complete (In Seconds) = %d"), TimeToComplete));
    		LerpTimer = (TimeSinceStart/TimeToComplete) * SplineLength;
    
    		if (LerpTimer < 0)
    		{
    			LerpTimer = 0;
    		}
    		else if (LerpTimer >= SplineLength)
    		{
    			LerpTimer -= SplineLength;
    			TimeSinceStart = 0;
    		}
    		
    		SetActorTransform(TrackSplineRef->GetTransformAtDistanceAlongSpline(LerpTimer, ESplineCoordinateSpace::World, false));
    	}
}

bool ATrainEngine::ChangeTrack(AActor* NewTrack)
{
	if (USplineComponent* TempSplineRef = Cast<USplineComponent>(NewTrack->GetRootComponent()); TempSplineRef == nullptr)
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