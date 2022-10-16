// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_TrackConnector.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATest_TrackConnector::ATest_TrackConnector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Might need a do once gate here to make sure that the spline only populates the first time it's dragged into scene

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Comp"));
	Spline->SetupAttachment(RootComponent);

	Start = CreateDefaultSubobject<UBoxComponent>(TEXT("Start TriggerBox"));
	Start->SetupAttachment(Spline);

	Final = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox #2"));
	Final->SetupAttachment(Spline);

	// The issue is that these locations aren't determined until after the construction script runs
	// Perhaps some dummy scene components can be an analogue?
	Start->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::Local));
	Final->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::Local));
}

// Called when the game starts or when spawned
void ATest_TrackConnector::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(5, 5, FColor::Orange, TEXT("Starting BeginPlay of " + this->GetName()));

	TArray<AActor*> OverlappingActorArray;
	Start->GetOverlappingActors(OverlappingActorArray);

	if (OverlappingActorArray.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Orange, TEXT("Overlapping Actors Detected"));
		for (int i = 0; i < OverlappingActorArray.Num(); i++)
		{
			if (OverlappingActorArray[i]->GetClass() == this->GetClass())
			{
				GEngine->AddOnScreenDebugMessage(11, 5, FColor::Orange, TEXT("Spline Detected"));
				USplineComponent* OtherSpline = Cast<USplineComponent>(OverlappingActorArray[i]);
				
				if (OtherSpline == nullptr)
				{
					continue;
				}

				// Determines whether or not the world position of the end of the spline is within the bounds of the start detection box

				if (UKismetMathLibrary::IsPointInBox(
					OtherSpline->GetLocationAtSplinePoint(OtherSpline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World),
					Start->GetComponentLocation(),
					Start->GetLocalBounds().GetBox().GetExtent()
				))
				{
					GEngine->AddOnScreenDebugMessage(12, 5, FColor::Orange, TEXT("Spline starting to move"));
					// Sets the world location of the end spline point
					OtherSpline->SetLocationAtSplinePoint(
						OtherSpline->GetNumberOfSplinePoints() - 1,
						Start->GetComponentLocation(),
						ESplineCoordinateSpace::World
						);

					GEngine->AddOnScreenDebugMessage(13, 5, FColor::Orange, TEXT("Setting spline leave tangent"));
					// Sets the arrive tangent of the moved point to be the negation of the leave tangent of the Start of the next spline
					OtherSpline->SetTangentsAtSplinePoint(
						OtherSpline->GetNumberOfSplinePoints() - 1,
						Spline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World) * -1,
						OtherSpline->GetLeaveTangentAtSplinePoint(OtherSpline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World),
						ESplineCoordinateSpace::World
						);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(11, 2, FColor::Orange,
						TEXT("The End of the other spline isn't inside the spline box"));
					continue;
				}
				
				break;
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(-2, 5, FColor::Orange, TEXT("Start Processed"));

	Final->GetOverlappingActors(OverlappingActorArray);

	if (OverlappingActorArray.Num() > 0)
	{
		
	}

	GEngine->AddOnScreenDebugMessage(5, 5, FColor::Orange, TEXT("Ending BeginPlay of " + this->GetName()));
}

// Called every frame
void ATest_TrackConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATest_TrackConnector::BeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	UBoxComponent* BoxComponent = Cast<UBoxComponent>(OverlappedComponent);

	if (BoxComponent == nullptr)
	{
		return;
	}

	if (BoxComponent == Start)
	{
		if (OtherActor->GetClass() == Spline->GetClass())
		{
			GEngine->AddOnScreenDebugMessage(1, 2, FColor::Green, TEXT("Start: Detected Other Track Actor"));
			
			if (OtherComp->GetClass() == Start->GetClass())
			{
				GEngine->AddOnScreenDebugMessage(2, 2, FColor::Blue, TEXT("Start: Detected Other Track Component"));
			}
		}
	}
	else if (BoxComponent == Final)
	{
		if (OtherActor->GetClass() == Spline->GetClass())
		{
			GEngine->AddOnScreenDebugMessage(1, 2, FColor::Green, TEXT("End: Detected Other Track Actor"));
			
			if (OtherComp->GetClass() == Start->GetClass())
			{
				GEngine->AddOnScreenDebugMessage(2, 2, FColor::Blue, TEXT("End: Detected Other Track Component"));
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, TEXT("The Component Didn't really connect right"));
	}
}
