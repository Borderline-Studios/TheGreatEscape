// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainEngine.h"
#include "SplineTrack.h"
#include "Kismet/KismetMathLibrary.h"

// Static Variable Declarations
TStaticArray<UStaticMesh*, 4> ATrainEngine::StaticMeshRefs;

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

    for (int i = 0; i < 4; i++)
    {
	    if (!StaticMeshRefs[i])
	    {
	    	UStaticMesh* Mesh = nullptr;

	        if (i == 0)
	        {
	        	const ConstructorHelpers::FObjectFinder<UStaticMesh> FirstCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Base_Full_Train_Car_Base_Full.Train_Car_Base_Full_Train_Car_Base_Full'"));
	        	Mesh = FirstCarMesh.Object;
	        }
	    	else if (i == 1)
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> SecondCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_FlatBed_Full_Train_Car_FlatBed_Full.Train_Car_FlatBed_Full_Train_Car_FlatBed_Full'"));
	    		Mesh = SecondCarMesh.Object;
	        }
	    	else if (i == 2)
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> ThirdCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Weapons_Full_Train_Car_Weapon_Full.Train_Car_Weapons_Full_Train_Car_Weapon_Full'"));
	    		Mesh = ThirdCarMesh.Object;
	        }
	    	else if (i == 3)
	        {
	    		const ConstructorHelpers::FObjectFinder<UStaticMesh> FourthCarMesh(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Windows_Full.Train_Car_Windows_Full'"));
	    		Mesh = FourthCarMesh.Object;
	        }

	    	StaticMeshRefs[i] = Mesh;
	    }
    }
}

// Called when the game starts or when spawned
void ATrainEngine::BeginPlay()
{
	Super::BeginPlay();

	// Initialising variables
	TimeSinceStart = 0.0f;
	bHasStartedMoving = false;
	SplineLength = -5;

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
		if (TrackSplineRef)
		{
			bHasStartedMoving = true;
			isTrainMoving = true;
		}
	}, (StartDelayTime >= 1) ? StartDelayTime : 0.1f, false);

	//GEngine->AddOnScreenDebugMessage(2, 5, FColor::Blue, FString::Printf(TEXT("Train Has Started Counting, will begin moving in %d Seconds"), StartDelayTime));

	// Spawning in the Carriages // Doesn't fire if CarriageCount <= 0
	for (int i = 0; i < CarriageCount; i++)
	{
		ATrainCarriage* TempRef = Cast<ATrainCarriage>(GetWorld()->SpawnActor(ATrainCarriage::StaticClass()));
		TempRef->InitialiseFromEngine(i, StaticMeshRefs[i%4], TrackSplineRef);
		
		CarriageRefs.Push(TempRef);
	}

	EngineStart = ((CarriageCount >= 0) ? CarriageCount : 0) * 1500;
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
    		TimeSinceStart += (DeltaTime * TrainSpeedModifier);
    	}
    	
	    const float TimerTrack = TimeSinceStart / TimeToComplete;

    	const float CurrentSplineProgress = FMath::Lerp(0, SplineLength, TimerTrack);

    	SetActorLocation(TrackSplineRef->GetLocationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World));
    	SetActorRotation(TrackSplineRef->GetRotationAtDistanceAlongSpline(CurrentSplineProgress, ESplineCoordinateSpace::World));

        for (int i = 0; i < CarriageRefs.Num(); i++)
        {
	        CarriageRefs[i]->ProcessMovement(CurrentSplineProgress);
        }
    	
    	if (!UKismetMathLibrary::InRange_FloatFloat(TimerTrack, 0.0, 1.0, false, true))
    	{
    		TimeSinceStart = 0;
    	}
    
    	// This code seems expensive. If we can move it to the carriages then it might be more effective.
    	// As it stands, it runs and keeps the train moving through the spline.
    	// However, it causes each train segment to hang on the point of track change.
    	// Might be an issue with resetting the train on the new spline but it seems weird that it happens with each carriage.
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

void ATrainEngine::SetTrainSpeed(TrainSpeed NewSpeed)
{
	switch (NewSpeed)
	{
	case TrainSpeed::Slow:
		TrainSpeedModifier = 0.5f;
		break;
	case TrainSpeed::Standard:
		TrainSpeedModifier = 1.0f;
		break;
	case TrainSpeed::Fast:
		TrainSpeedModifier = 2.0f;
		break;
	default:
		TrainSpeedModifier = 1.0f;
		break;
	}
}