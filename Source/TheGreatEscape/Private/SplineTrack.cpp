// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : SplineTrack.cpp
// Description : Contains the implementation of the SplineTrack c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "SplineTrack.h"
#include "TrainEngine.h"
#include "Kismet/KismetMathLibrary.h"

/**
 * @brief Sets default values 
 */
ASplineTrack::ASplineTrack()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Populate and set up the Spline Component
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Comp"));
	Spline->SetupAttachment(RootComponent);
	Spline->SetUnselectedSplineSegmentColor(FLinearColor(FColor::Blue));
	Spline->SetSelectedSplineSegmentColor(FLinearColor(FColor::Magenta));
	Spline->SetTangentColor(FLinearColor(0.718f, 0.589f, 0.921f, 1.0f));

	// Populate and set up the start collision component
	Start = CreateDefaultSubobject<UBoxComponent>(TEXT("Start TriggerBox"));
	Start->SetupAttachment(Spline);
	Start->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::Local));

	// Populate and set up the end collision component
	Final = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox #2"));
	Final->SetupAttachment(Spline);
	Final->SetWorldScale3D(FVector(0.1f));
	Final->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::Local));
}

/**
 * @brief Called when the game starts or when spawned
 * Was once used to set up snapping of track pieces at the start of runtime but I have since swapped to another method.
 */
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
}

/**
 * @brief Called every frame
 * @param DeltaTime Time between frames
 */
void ASplineTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if final is not the same as the location of the 
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

/**
 * @brief Allows for the train reference to be populated externally but only once. If a new reference is attempted after the first, it will not apply.
 * @param NewTrainRef The train engine for the spline to reference.
 */
void ASplineTrack::PopulateTrainRef(ATrainEngine* NewTrainRef)
{
	// Check for if the train reference has not been populated 
	if (!TrainRef)
	{
		// Populate the train reference
		TrainRef = NewTrainRef;
	}
}

/**
 * @brief Exposes the GetSplineComponent Function to code.
 * @return 
 */
USplineComponent* ASplineTrack::GetSpline() const
{
	return GetSplineComponent();
}

/**
 * @brief Allows external access to the segment after this spline's segment.
 * @return Will return the next spline if populated or a nullptr if not.
 */
ASplineTrack* ASplineTrack::GetNextSpline()
{
	return NextSpline ? NextSpline : nullptr;
}

/**
 * @brief Allows external access to this segment's spline component through blueprints only
 * @return This segment's spline component
 */
USplineComponent* ASplineTrack::GetSplineComponent() const
{
	return Spline;
}

/**
 * @brief Allows external access to the Box Collision component at the beginning of the spline segment.
 * @return The box collider at the start of the spline segment
 */
UBoxComponent* ASplineTrack::GetStartBoxCollider() const
{
	return Start;
}

#if WITH_EDITOR
/**
 * @brief Editor Function that allows for certain actions to be performed at certain stages during movement.
 * This function sets the WorldLocation of both Box Collision components so that they move with the start and end of the spline respectively.
 * @param bFinished Whether the movement has finished or not. Can be used to make things happen only while moving or not moving.
 */
void ASplineTrack::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	// Set the location of the start and end components to the current position of the start and end of the spline
	Start->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	Final->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
}

/**
 * @brief Editor function that allows for certain actions to be performed when undo-ing an editor action involving this object
 * This function sets the location of both Box Colliders to the start and end of the spline component respectively.
 */
void ASplineTrack::PostEditUndo()
{
	Super::PostEditUndo();

	// Set the location of the start and end components to the current position of the start and end of the spline
	Start->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	Final->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
}
#endif // WITH_EDITOR