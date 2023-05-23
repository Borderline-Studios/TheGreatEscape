// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveElevator.cpp
// Description : Contains the implementation of the ObjectiveElevator class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "Objectives/ObjectiveElevator.h"
#include "Kismet/KismetMathLibrary.h"

/**
 * @brief Sets default values
 */
AObjectiveElevator::AObjectiveElevator()
{
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

/**
 * @brief Called when the game starts or when spawned
 */
void AObjectiveElevator::BeginPlay()
{
    Super::BeginPlay();

    // Setting up start and end positions of the elevator
    StartPos = GetActorLocation();
    EndPos = StartPos + FVector(0.0f, 0.0f, 5560.0);

    // Timelines need curves. without them they don't function
    if (MovementCurve)
    {
        // Creating a bridge between the timeline and the function delegate for processing the timeline
        FOnTimelineFloat ProgressFunction;
        ProgressFunction.BindUFunction(this, TEXT("ProcessMovement"));
        MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);

        // Creating a bridge between the timeline and the function delegate for when the timeline ends
        FOnTimelineEvent OnTimelineFinishedFunction;
        OnTimelineFinishedFunction.BindUFunction(this, TEXT("OnEndMovementTimeline"));
        MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinishedFunction);

        // Initialising the timeline
        MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
        const float FloatToComplete = TimeToComplete;
        MovementTimeline.SetPlayRate(1.0f/FloatToComplete);
        MovementTimeline.SetLooping(false);
    }
}

/**
 * @brief Called when the game ends or lifetime of the instance is coming to an end
 * @param EndPlayReason Reason for why the function is being called
 */
void AObjectiveElevator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // Clearing the timer handle on EndPlay so that it
    // doesn't get called when it can't be processed, like when a new world is loaded.
    GetWorldTimerManager().ClearTimer(TimelineEndHandle);
}

/**
 * @brief Called every frame
 * @param DeltaTime The difference in time between frames
 */
void AObjectiveElevator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // We only want to tick along the timeline if it's currently playing
    if (MovementTimeline.IsPlaying())
    {
        MovementTimeline.TickTimeline(DeltaTime);
    }
}

/**
 * @brief Timeline delegate function. Called while the timeline is playing
 * @param TimelineProgress The float that is being incremented throughout the timeline.
 */
void AObjectiveElevator::ProcessMovement(float TimelineProgress)
{
    // Determining the position of the elevator using the start and end positions
    // as well as TimelineProgress as the alpha between them.
    const FVector CurrentPos = UKismetMathLibrary::VLerp(StartPos, EndPos, TimelineProgress);

    // Setting the elevators position using the above calculated position
    SetActorLocation(CurrentPos);
}

/**
 * @brief Timeline delegate function. Called at the end of processing the timeline
 * Restarts the movement process
 */
void AObjectiveElevator::OnEndMovementTimeline()
{
    // Sets a timer to repeat the movement process after a set amount of time.
    GetWorldTimerManager().SetTimer(TimelineEndHandle, [&]()
    {
        bElevatingUp = !bElevatingUp;
        DetermineDirection();
    }, TimeBetweenElevations, false);
}

/**
 * @brief Determines which direction to move the elevator based on a tracking boolean
 */
void AObjectiveElevator::DetermineDirection()
{
    // Checks to see if the elevator has been started using EnableElevator()
    if (bElevatorStarted)
    {
        // Checks to see which direction we need to move.
        // The elevator starts in the lowered position and ends in the raised position
        if (bElevatingUp)
        {
            MovementTimeline.PlayFromStart();
        }
        else
        {
            MovementTimeline.ReverseFromEnd();
        }
    }
}

/**
 * @brief Enables the elevator and then tells it to start
 * This function is public and is called from the ObjectiveGate class
 * @param SlotsFilled The number of battery slots that have been filled
 */
void AObjectiveElevator::EnableElevator(int SlotsFilled)
{
    // Checks to make sure we're not already started
    // and that we've provided the right number of batteries.
    if (!bElevatorStarted && SlotsFilled >= BatteriesRequiredForActivation)
    {
        bElevatorStarted = true;
        DetermineDirection();
    }
}