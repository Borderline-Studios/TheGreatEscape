// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineTrack.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASplineTrack::ASplineTrack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Might need a do once gate here to make sure that the spline only populates the first time it's dragged into scene
	// Creating and attaching components
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Comp"));
	Spline->SetupAttachment(RootComponent);

	Start = CreateDefaultSubobject<UBoxComponent>(TEXT("Start TriggerBox"));
	Start->SetupAttachment(Spline);

	Final = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox #2"));
	Final->SetupAttachment(Spline);
	Final->SetWorldScale3D(FVector(0.1f));
	
	Start->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::Local));
	Final->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::Local));

	// Editing component variables
	Spline->SetUnselectedSplineSegmentColor(FLinearColor(FColor::Blue));
	Spline->SetSelectedSplineSegmentColor(FLinearColor(FColor::Magenta));
	Spline->SetTangentColor(FLinearColor(0.718f, 0.589f, 0.921f, 1.0f));

	//Final->OnComponentBeginOverlap.AddDynamic(this, &ASplineTrack::OnFinalBeginOverlap);
}

// Called when the game starts or when spawned
void ASplineTrack::BeginPlay()
{
	Super::BeginPlay();

	// DEBUG
	//Final->OnComponentBeginOverlap.AddDynamic(this, &ASplineTrack::OnFinalBeginOverlap);

	// It works but only if the second BP of the item is used as the start instead of the first.
	// Seems one-directional, either need to reverse order or sort something else out...
	TArray<AActor*> OverlappingActorArray;
	// Handles the Start Collision -> Placing the second in front of the first
	Start->GetOverlappingActors(OverlappingActorArray);

	// Suspect that populating the NextSpline from here fails due to pointers being depopulated once the OverlappingActors array is overwritten
	if (!OverlappingActorArray.IsEmpty())
	{
		for (int i = 0; i < OverlappingActorArray.Num(); ++i)
		{
			if (OverlappingActorArray[i]->GetClass() == this->GetClass())
			{
				ASplineTrack* temp = Cast<ASplineTrack>(OverlappingActorArray[i]);

				if (temp == nullptr)
				{
					continue;
				}

				USplineComponent* OtherSpline = temp->GetSplineComponent();
				const int EndPointIndex = OtherSpline->GetNumberOfSplinePoints() - 1;

				if (UKismetMathLibrary::IsPointInBox(
					OtherSpline->GetLocationAtSplinePoint(EndPointIndex, ESplineCoordinateSpace::World),
					Start->GetComponentLocation(),
					Start->GetScaledBoxExtent()
				))
				{
					// Set the location of the spline point
					OtherSpline->SetWorldLocationAtSplinePoint(EndPointIndex, Start->GetComponentLocation());

					// Update the tangent of the spline point to match
					OtherSpline->SetTangentsAtSplinePoint(
						EndPointIndex,
						Spline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						Spline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						ESplineCoordinateSpace::World
						);
					Spline->SetTangentsAtSplinePoint(
						0,
						Spline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						Spline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						ESplineCoordinateSpace::World
						);

					//GEngine->AddOnScreenDebugMessage(10, 3.0, FColor::Green, TEXT("Detection and Snapping Succeeded Start"));
				}
				else
				{
					//GEngine->AddOnScreenDebugMessage(10, 3.0, FColor::Red, TEXT("Oof"));
					continue;
				}

				break;
			}
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(10, 3.0, FColor::Orange, TEXT("Detection and Snapping Failed Start"));
	}

	// Handles the End Collision -> Placing the second behind the first
	Final->GetOverlappingActors(OverlappingActorArray);
	
	if (!OverlappingActorArray.IsEmpty())
	{
		for (int i = 0; i < OverlappingActorArray.Num(); ++i)
		{
			if (OverlappingActorArray[i]->GetClass() == this->GetClass())
			{
				ASplineTrack* temp = Cast<ASplineTrack>(OverlappingActorArray[i]);

				if (temp == nullptr)
				{
					continue;
				}

				USplineComponent* OtherSpline = temp->GetSplineComponent();
				const int ThisEndPointIndex = Spline->GetNumberOfSplinePoints() - 1;

				if (UKismetMathLibrary::IsPointInBox(
					Spline->GetLocationAtSplinePoint(ThisEndPointIndex, ESplineCoordinateSpace::World),
					temp->GetStartBoxCollider()->GetComponentLocation(),
					temp->GetStartBoxCollider()->GetScaledBoxExtent()
				))
				{
					// Set the location of the spline point
					Spline->SetWorldLocationAtSplinePoint(ThisEndPointIndex, temp->GetStartBoxCollider()->GetComponentLocation());

					// Update the tangent of the spline point to match
					Spline->SetTangentsAtSplinePoint(
						ThisEndPointIndex,
						OtherSpline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						OtherSpline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						ESplineCoordinateSpace::World
						);
					OtherSpline->SetTangentsAtSplinePoint(
						0,
						OtherSpline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						OtherSpline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						ESplineCoordinateSpace::World
						);

					//GEngine->AddOnScreenDebugMessage(11, 3.0, FColor::Green, TEXT("Detection and Snapping Succeeded End"));
				}
				else
				{
					//GEngine->AddOnScreenDebugMessage(11, 3.0, FColor::Red, TEXT("Oof"));
				}
				break;
			}
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(11, 3.0, FColor::Orange, TEXT("Detection and Snapping Failed End"));
	}

	const FTransform FinalSplinePoint = Spline->GetTransformAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World);
	// GEngine->AddOnScreenDebugMessage(50, 20.0, FColor::Red, TEXT("Initial Location: " + FinalSplinePoint.GetLocation().ToString()));
	//GEngine->AddOnScreenDebugMessage(FMath::Rand() * 40, 120.0, FColor::Red, FString::Printf(TEXT("Spline " + GetName() + " length: %d"), Spline->GetSplineLength()));
}

// Called every frame
void ASplineTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Final->GetComponentLocation() != Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World))
	{
		const FTransform FinalSplinePoint = Spline->GetTransformAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World);
		// GEngine->AddOnScreenDebugMessage(51, 20.0, FColor::Red, TEXT("New Location: " + FinalSplinePoint.GetLocation().ToString()));
		Final->SetWorldLocation(FinalSplinePoint.GetLocation());
		Final->SetWorldRotation(FinalSplinePoint.GetRotation());

		if (!NextSpline)
		{
			// Handles getting the data for the next spline in the sequence
			TArray<AActor*> OverlappingActorArray;
			Final->GetOverlappingActors(OverlappingActorArray);

			if (!OverlappingActorArray.IsEmpty())
			{
				for (int i = 0; i < OverlappingActorArray.Num(); ++i)
				{
					if (OverlappingActorArray[i]->GetClass() == this->GetClass())
					{
						NextSpline = Cast<ASplineTrack>(OverlappingActorArray[i]);
						//GEngine->AddOnScreenDebugMessage(110, 5.0f, FColor::Magenta, TEXT("NextSpline Populated"));
						break;
					}
				}
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(110, 5.0f, FColor::Magenta, TEXT("NextSpline Failed to populate"));
			}
		}
	}
}

void ASplineTrack::PopulateTrainRef(ATrainEngine* NewTrainRef)
{
	if (TrainRef == nullptr)
	{
		TrainRef = NewTrainRef;
	}
}

USplineComponent* ASplineTrack::GetSpline()
{
	return Spline;
}

ASplineTrack* ASplineTrack::GetNextSpline()
{
	return NextSpline ? NextSpline : nullptr;
}

/*void ATest_TrackConnector::BeginOverlap(
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
	
}*/

USplineComponent* ASplineTrack::GetSplineComponent() const
{
	return Spline;
}

UBoxComponent* ASplineTrack::GetStartBoxCollider() const
{
	return Start;
}

void ASplineTrack::OnFinalBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(99, 1.0f, FColor::Blue, TEXT("Overlapped Comp: " + OverlappedComponent->GetName()));

	return;
	if (OtherActor == Cast<AActor>(TrainRef))
	{
		if (TrainRef->GetTrackOverrideState()) {return;}
		// if (TrainRef->GetTrackOverrideState()) {return;}
		//GEngine->AddOnScreenDebugMessage(100, 1.0f, FColor::Green, TEXT("Collision Starting With Train On Final Overlap"));

		TrainRef->ShouldChangeTracks = true;

		if (NextSpline)
		{
			TrainRef->ChangeTrack(NextSpline);
		}
		
		//GEngine->AddOnScreenDebugMessage(101, 1.0f, FColor::Green, TEXT("Collision Ending With Train On Final Overlap"));
	}
}

#if WITH_EDITOR
void ASplineTrack::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	// Code that runs during the move action as well as one more time at the end of that action:
	Start->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	Final->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));

	// Code only runs here during specific stages of the Editor Move
	if (!bFinished)	// Only runs during the actual moving of the item
	{
		
	}
	else			// Only runs at the end of the Move action
	{
		
	}
}

void ASplineTrack::PostEditUndo()
{
	Super::PostEditUndo();

	Start->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	Final->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
}
#endif // WITH_EDITOR

/*
// Start->GetOverlappingActors(OverlappingActorArray);

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
*/

/*
TArray<AActor*> OverlappingActorArray;
	Final->GetOverlappingActors(OverlappingActorArray);
	
	if (!OverlappingActorArray.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Orange, TEXT("Overlapping Actors Detected"));
		for (int i = 0; i < OverlappingActorArray.Num(); i++)
		{
			GEngine->AddOnScreenDebugMessage(20 + i, 5, FColor::Cyan, TEXT("Name: " + OverlappingActorArray[i]->GetName()));

			if (OverlappingActorArray[i]->GetClass() == this->GetClass())
			{
				GEngine->AddOnScreenDebugMessage(11, 5, FColor::Orange, TEXT("Detected Spline"));
				ASplineTrack* OtherTrack = Cast<ASplineTrack>(OverlappingActorArray[i]);
				USplineComponent* OtherSpline = OtherTrack->GetSplineComponent();
				// USplineComponent* OtherSpline = Cast<USplineComponent>(OverlappingActorArray[i]);
				
				if (OtherSpline == nullptr)
				{
					GEngine->AddOnScreenDebugMessage(11, 5, FColor::Red, TEXT("Spline Detection Failed"));
					continue;
				}

				// Determines whether or not the world position of the end of the spline is within the bounds of the End/Final detection box

				if (UKismetMathLibrary::IsPointInBox(
					OtherSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World),
					Final->GetComponentLocation(),
					Final->GetScaledBoxExtent()
				))
				{
					GEngine->AddOnScreenDebugMessage(12, 5, FColor::Orange, TEXT("Spline starting to move"));
					// Sets the world location of the end spline point
					OtherSpline->SetLocationAtSplinePoint(
						0,
						Final->GetComponentLocation(),
						ESplineCoordinateSpace::World
						);

					GEngine->AddOnScreenDebugMessage(13, 5, FColor::Orange, TEXT("Setting spline arrive tangent"));
					// Sets the arrive tangent of the moved point to be the negation of the leave tangent of the Start of the next spline
					OtherSpline->SetTangentsAtSplinePoint(
						0,
						OtherSpline->GetArriveTangentAtSplinePoint(0, ESplineCoordinateSpace::World),
						Spline->GetArriveTangentAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World),
						ESplineCoordinateSpace::World
						);

					GEngine->AddOnScreenDebugMessage(14, 5, FColor::Orange,
						TEXT("Arrive Tangent" + Spline->GetArriveTangentAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World).ToString()));
					GEngine->AddOnScreenDebugMessage(15, 5, FColor::Orange,
						TEXT("Leave Tangent" + OtherSpline->GetLeaveTangentAtSplinePoint(0, ESplineCoordinateSpace::World).ToString()));
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
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Orange, TEXT("No Overlapping Actors"));
	}

	GEngine->AddOnScreenDebugMessage(7, 5, FColor::Orange, TEXT("Ending BeginPlay of " + this->GetName()));
 */