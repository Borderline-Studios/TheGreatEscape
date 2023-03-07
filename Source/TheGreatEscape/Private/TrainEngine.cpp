// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainEngine.h"
#include "SplineTrack.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATrainEngine::ATrainEngine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	BoxComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Carriage Mesh"));
	BoxComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	BoxComp->SetStaticMesh(MeshObj.Object);
	BoxComp->SetWorldScale3D(FVector(2.5f, 1.0f, 0.7f));

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComp->SetupAttachment(RootComponent);
	ArrowComp->SetArrowColor(FColor::Purple);
	ArrowComp->SetHiddenInGame(false);
	ArrowComp->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
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

	// Checking for a spline reference and connecting if one is detected
	if (!TrackActorRef) {}
	else if (USplineComponent* TempSplineRef = Cast<USplineComponent>(TrackActorRef->GetRootComponent()); TempSplineRef != nullptr)
	{
		TrackSplineRef = TempSplineRef;
	}

	if (TrackSplineRef)
	{
		//GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, TEXT("Train Spline Ref populated"));
		SplineLength = TrackSplineRef->GetSplineLength();

		SetActorLocation(TrackSplineRef->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
		SetActorRotation(TrackSplineRef->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	}

	FTimerHandle TrainStartHandle;
	GetWorld()->GetTimerManager().SetTimer(TrainStartHandle, [&]()
	{
		bHasStartedMoving = true;
		isTrainMoving = true;
	}, StartDelayTime, false);

	//GEngine->AddOnScreenDebugMessage(2, 5, FColor::Blue, FString::Printf(TEXT("Train Has Started Counting, will begin moving in %d Seconds"), StartDelayTime));
}

// Called every frame
void ATrainEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Standard Tick Operation
    if (bHasStartedMoving)
    {
    	if (isTrainMoving)
    	{
    		TimeSinceStart += DeltaTime;
    	}
    	
	    const float TimerTrack = TimeSinceStart / TimeToComplete;

    	const float CurrentSplineProgress = FMath::Lerp(0, SplineLength, TimerTrack);

    	SetActorLocation(TrackSplineRef->GetLocationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World));
    	SetActorRotation(TrackSplineRef->GetRotationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World));
    	
    	if (!UKismetMathLibrary::InRange_FloatFloat(TimerTrack, 0.0, 1.0, false, true))
    	{
    		TimeSinceStart = 0;
    	}
    
    	// // This code seems expensive. If we can move it to the carriages then it might be more effective.
    	// // As it stands, it runs and keeps the train moving through the spline.
    	// // However, it causes each train segment to hang on the point of track change.
    	// // Might be an issue with resetting the train on the new spline but it seems weird that it happens with each carriage.
    	// if (CarriageCount > 0)	
	    // {
		   //  for (int i = 0; i < CarriageRefs.Num(); i++)
		   //  {
		   //  	int NewSplineIndex = 0;
		   //  	const float CarriageTimerTrack = TimerTrack - ((i + 1) * (CompleteSplineLength * DistanceBetweenCars / 100) / CompleteSplineLength);
		   //  
		   //  	for (int j = CompleteTrackRefs.Num() - 1; j > 0; j--)
		   //  	{
		   //  		if (UKismetMathLibrary::InRange_FloatFloat(CarriageTimerTrack, SplineTravelParameters[j - 1].TimeToSwap, SplineTravelParameters[j].TimeToSwap, true, false))
		   //  		{
		   //  			NewSplineIndex = j;
		   //  			break;
		   //  		}
		   //  	}
	    //
		   //  	if (CarriageRefs[i]->CurrentSplineIndex != NewSplineIndex)
		   //  	{
		   //  		//GEngine->AddOnScreenDebugMessage(5000 + i, 1, FColor::Red, FString::Printf(TEXT("Carriage %d changes track"), i + 1));
		   //  		CarriageRefs[i]->CurrentSplineIndex = NewSplineIndex;
		   //  		CarriageRefs[i]->ChangeTrack(CompleteTrackRefs[CarriageRefs[i]->CurrentSplineIndex]);
		   //  	}
	    //
		   //  	float TimeOffset = 0;
	    //
		   //  	for (int j = 0; j < CarriageRefs[i]->CurrentSplineIndex; j++)
		   //  	{
		   //  		TimeOffset += SplineTravelParameters[j].TimeToTraverse;
		   //  	}
	    //
		   //  	const float CarriageSplineProgress = FMath::Lerp(EngineStart, CompleteSplineLength, CarriageTimerTrack - TimeOffset);
	    //
		   //  	CarriageRefs[i]->SetActorLocation(CarriageRefs[i]->TrackSplineRef->GetLocationAtDistanceAlongSpline(CarriageSplineProgress, ESplineCoordinateSpace::World));
		   //  	CarriageRefs[i]->SetActorRotation(CarriageRefs[i]->TrackSplineRef->GetRotationAtDistanceAlongSpline(CarriageSplineProgress, ESplineCoordinateSpace::World));
		   //  }
	    // }
    }
}

void ATrainEngine::ToggleTrainStop()
{
	isTrainMoving = !isTrainMoving;
}

// bool ATrainEngine::ChangeTrack(AActor* NewTrack)
// {
// 	if (USplineComponent* TempSplineRef = Cast<USplineComponent>(NewTrack->GetRootComponent()); TempSplineRef == nullptr || ShouldChangeTracks == false)
// 	{
// 		return false;
// 	}
// 	else
// 	{
// 		TrackSplineRef = TempSplineRef;
// 	}
//
// 	//TimeSinceStart = 0;
// 	//SplineLength = TrackSplineRef->GetSplineLength();
//
// 	return true;
// }
//
// void ATrainEngine::GetSplineReferences(TArray<ASplineTrack*>& Array)
// {
// 	Array = CompleteTrackRefs;
// }
//
// void ATrainEngine::BPOverrideTrack(AActor* TrackOverride)
// {
// 	if (USplineComponent* TempSplineRef = Cast<USplineComponent>(TrackOverride->GetRootComponent()); TempSplineRef != nullptr)
// 	{
// 		TrackSplineRef = TempSplineRef;
// 	}
//
// 	if (TrackSplineRef)
// 	{
// 		//GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("Train Spline Ref Overridden"));
// 		SplineLength = TrackSplineRef->GetSplineLength();
// 	}
// 	
// 	EngineStart = FMath::Lerp(0.0f, SplineLength, 0.0f);
// 	SetActorTransform(TrackSplineRef->GetTransformAtSplinePoint(0, ESplineCoordinateSpace::World));
// 	isTrackOverridden = true;
// }
//
// bool ATrainEngine::GetTrackOverrideState()
// {
// 	return isTrackOverridden;
// }