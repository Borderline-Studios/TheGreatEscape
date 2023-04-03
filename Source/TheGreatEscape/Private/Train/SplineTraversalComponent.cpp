// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Train/SplineTraversalComponent.h"
#include "SplineTrack.h"

// Sets default values for this component's properties
USplineTraversalComponent::USplineTraversalComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Get a reference to the attached parent
	OwnerRef = GetOwner();
}


// Called when the game starts
void USplineTraversalComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (SplineParent)
	{
		// Spline
		SplineRef = SplineParent->GetSpline();
		SplineLength = SplineRef->GetSplineLength();

		// Timeline
		if (MovementCurve)
		{
			OwnerRef->GetRootComponent()->SetMobility(EComponentMobility::Movable);

			FOnTimelineFloat ProgressFunction;
			ProgressFunction.BindUFunction(this, TEXT("ProcessMovement"));
			MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);
		
			FOnTimelineEvent OnTimelineFinishedFunction;
			OnTimelineFinishedFunction.BindUFunction(this, TEXT("OnEndMovementTimeline"));
			MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinishedFunction);
		
			MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
			const float floatToComplete = TimeToComplete;
			MovementTimeline.SetPlayRate(1.0f/floatToComplete);
			MovementTimeline.SetLooping(bLoop);
			MovementTimeline.Play();
		}

		Activate();
	}
}


// Called every frame
void USplineTraversalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!SplineRef) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}

	// ...
}

void USplineTraversalComponent::ProcessMovement(const float TimelineProgress) const
{
	if (!SplineRef) return;
	
	const FVector CurrentSplineLoc = SplineRef->GetLocationAtDistanceAlongSpline(TimelineProgress * SplineLength, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRot = SplineRef->GetRotationAtDistanceAlongSpline(TimelineProgress * SplineLength, ESplineCoordinateSpace::World);
	CurrentSplineRot.Roll = 0;
	
	OwnerRef->SetActorLocationAndRotation(CurrentSplineLoc, CurrentSplineRot);
}

void USplineTraversalComponent::OnEndMovementTimeline()
{
}

