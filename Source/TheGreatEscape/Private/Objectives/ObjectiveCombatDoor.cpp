// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveCombatDoor.cpp
// Description : Contains the implementation of the ObjectiveCombatDoor c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "Objectives/ObjectiveCombatDoor.h"

#include "Character/Player/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
// TODO: Add mechanism for preventing it from re-locking after the objective has been complete
/**
 * @brief Sets default values
 */
AObjectiveCombatDoor::AObjectiveCombatDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Populate and set up the root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = Root;

	// Populate and set up the arrow component
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Indicator"));
	Arrow->SetupAttachment(RootComponent);

	// Populate and set up the static mesh component of the frame and the door
	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	Frame->SetupAttachment(RootComponent);
	
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(RootComponent);
	DoorStartPosition = Door->GetComponentLocation();

	// Populate and set up of the player detection component, including setting function delegates
	PlayerDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Detector"));
	PlayerDetector->SetupAttachment(RootComponent);
	PlayerDetector->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveCombatDoor::BeginDetectorOverlap);
	PlayerDetector->OnComponentEndOverlap.AddDynamic(this, &AObjectiveCombatDoor::EndDetectorOverlap);
}

/**
 * @brief Called when the game starts or when spawned
 */
void AObjectiveCombatDoor::BeginPlay()
{
	Super::BeginPlay();

	// Setting the Door Start Position to its current location
	DoorStartPosition = Door->GetComponentLocation();

	// Setting up position of the forward detection position.
	// This is used to determine which side the player is when they leave the detection component
	ForwardDetectionPosition = GetActorForwardVector() * 250;

	// Check to see if the static player reference has been populated yet. If it has, great, if not we need to populate.
	if (!PlayerRef)
	{
		// Try to populate it by searching for an actor of the player's class
		PlayerRef = UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass());
	}

	// Check to see if the movement curve has been populated.
	// The timeline won't work without it so if it doesn't have one there's no point in setting up the rest.
	if (MovementCurve)
	{
		// Creating a bridge between the timeline and the delegate function we're using
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, TEXT("ProcessMovement"));
		// Adding a float track to that function
		MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);

		// Initialising the rest of the timeline functionality
		MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
		const float FloatToComplete = DoorSFX->GetDuration();
		MovementTimeline.SetPlayRate(1.0f/FloatToComplete);
		MovementTimeline.SetLooping(false);
	}
}

/**
 * @brief Called when the game ends or lifetime of the instance is coming to an end
 * @param EndPlayReason The reason why the EndPlay function is being called
 */
void AObjectiveCombatDoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

/**
 * @brief Called Every frame. Allows for the timeline to process when it's activated
 * @param DeltaTime Change in time between frames
 */
void AObjectiveCombatDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We only want to tick the timeline if it's playing.
	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}
}

/**
 * @brief OnComponentBeginOverlap delegate function, used to detect whenever the player enters the space we want the door to open
 * @param OverlappedComponent The component of this actor being overlapped
 * @param OtherActor The actor being overlapped
 * @param OtherComp The component of the other actor being overlapped
 * @param OtherBodyIndex 
 * @param bFromSweep Whether the result is from a sweeping collision
 * @param SweepResult Result of the sweeping collision, if being used
 */
void AObjectiveCombatDoor::BeginDetectorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Check to see if the other actor is our player
	if (OtherActor == PlayerRef)
	{
		MovementTimeline.PlayFromStart();
	}
}

/**
 * @brief OnComponentEndoverlap delegate function, used to detect whenever the player has left the detection space
 * @param OverlappedComponent The component of this actor being overlapped
 * @param OtherActor The actor being overlapped
 * @param OtherComp The component of the other actor being overlapped
 * @param OtherBodyIndex 
 */
void AObjectiveCombatDoor::EndDetectorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	// Check to see if the other actor is our player
	if (OtherActor == PlayerRef)
	{
		MovementTimeline.ReverseFromEnd();
		// float DistanceFromFront = FVector::Dist(ForwardDetectionPosition, PlayerRef->GetActorLocation());
		// float DistanceFromBack = FVector::Dist(-ForwardDetectionPosition, PlayerRef->GetActorLocation());
		//
		// if (DistanceFromFront < DistanceFromBack && bDoorCanOpen)
		// {
		// 	LockDoor();
		// }
	}
}

/**
 * @brief Timeline delegate function. Sets the location of the door as it opens and closes
 * @param TimelineProgress The float that is being incremented throughout the timeline.
 */
void AObjectiveCombatDoor::ProcessTimeline(float TimelineProgress) const
{
	// Calculate the current door position using the start, an offset from that start, and an alpha as provided by the TimelineProgress float
	const FVector CurrentDoorPosition = FMath::Lerp(DoorStartPosition, DoorStartPosition + FVector(DoorMoveDistance, 0.0f, 0.0f), TimelineProgress);

	// Setting the door to that position. Everything is already calculated in relative space, hence the relative location
	Door->SetRelativeLocation(CurrentDoorPosition);
}

/**
 * @brief Function that stops the door from being opened. "Unlocked" by ReleaseDoor()
 * Is a Private function so that it can only happen once and internally only.
 */
void AObjectiveCombatDoor::LockDoor()
{
	bDoorCanOpen = false;
}

/**
 * @brief Function that allows the door to be reopened. Normally already locked by LockDoor() by the time this is being called.
 * Public function so that it can be called externally.
 */
void AObjectiveCombatDoor::ReleaseDoor()
{
	bDoorCanOpen = true;
}

