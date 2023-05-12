// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Objectives/ObjectiveElevator.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AObjectiveElevator::AObjectiveElevator()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AObjectiveElevator::BeginPlay()
{
    Super::BeginPlay();

    StartPos = GetActorLocation();
    EndPos = StartPos + FVector(0.0f, 0.0f, 5560.0);

    if (MovementCurve)
    {
        FOnTimelineFloat ProgressFunction;
        ProgressFunction.BindUFunction(this, TEXT("ProcessMovement"));
        MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);

        FOnTimelineEvent OnTimelineFinishedFunction;
        OnTimelineFinishedFunction.BindUFunction(this, TEXT("OnEndMovementTimeline"));
        MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinishedFunction);
        
        MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
        const float FloatToComplete = TimeToComplete;
        MovementTimeline.SetPlayRate(1.0f/FloatToComplete);
        MovementTimeline.SetLooping(false);
    }
}

void AObjectiveElevator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    GetWorldTimerManager().ClearTimer(TimelineEndHandle);
}

// Called every frame
void AObjectiveElevator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (MovementTimeline.IsPlaying())
    {
        MovementTimeline.TickTimeline(DeltaTime);
    }
}

void AObjectiveElevator::ProcessMovement(float TimelineProgress)
{
    const FVector CurrentPos = UKismetMathLibrary::VLerp(StartPos, EndPos, TimelineProgress);

    SetActorLocation(CurrentPos);
}

void AObjectiveElevator::OnEndMovementTimeline()
{
    GetWorldTimerManager().SetTimer(TimelineEndHandle, [&]()
    {
        bElevatingUp = !bElevatingUp;
        DetermineDirection();
    }, TimeBetweenElevations, false);
}

void AObjectiveElevator::DetermineDirection()
{
    if (bElevatingUp)
    {
        MovementTimeline.PlayFromStart();
    }
    else
    {
        MovementTimeline.ReverseFromEnd();
    }
}

void AObjectiveElevator::EnableElevator(int SlotsFilled)
{
    if (!bElevatorStarted && SlotsFilled >= BatteriesRequiredForActivation)
    {
        DetermineDirection();
        bElevatorStarted = true;
    }
}