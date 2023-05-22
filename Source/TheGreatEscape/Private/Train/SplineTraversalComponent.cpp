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

		SplineOffset = SplineOffset % SplineLength;
		NormalisedOffset = static_cast<float>(SplineOffset) / static_cast<float>(SplineLength);

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
			const float FloatToComplete = TimeToComplete;
			MovementTimeline.SetPlayRate(1.0f/FloatToComplete);
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

	int OffsetProgress = (NormalisedOffset + TimelineProgress) * SplineLength;
	
	if (OffsetProgress > SplineLength) OffsetProgress -= SplineLength;
	else if (OffsetProgress < 0) OffsetProgress += SplineLength;
	
	const FVector CurrentSplineLoc = SplineRef->GetLocationAtDistanceAlongSpline(OffsetProgress, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRot = SplineRef->GetRotationAtDistanceAlongSpline(OffsetProgress, ESplineCoordinateSpace::World);
	CurrentSplineRot -= FRotator(0.0f, 90.0f, 0.0f);
	CurrentSplineRot.Roll = 0;
	
	OwnerRef->SetActorLocationAndRotation(CurrentSplineLoc, CurrentSplineRot);
}

void USplineTraversalComponent::OnEndMovementTimeline()
{
}

#if WITH_EDITOR
void USplineTraversalComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Get the name of the property that changed
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Check if the property name that was changed matches the property we're looking for,
	// in this case the Spline Reference
	if (PropertyName == GET_MEMBER_NAME_CHECKED(USplineTraversalComponent, SplineParent))
	{
		// Check the Spline Parent is populated
		if (SplineParent)
		{
			SplineRef = SplineParent->GetSpline();
			SplineLength = SplineRef->GetSplineLength();
			SplineOffset = 0;
			
			ProcessMovement(0);
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(USplineTraversalComponent, SplineOffset))
	{
		// Check the Spline Reference is populated
		if (SplineRef)
		{
			SplineOffset = SplineOffset % SplineLength;
			NormalisedOffset = static_cast<float>(SplineOffset) / static_cast<float>(SplineLength);
			ProcessMovement(0);
		}
	}
}
#endif
