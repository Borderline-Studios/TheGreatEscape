// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveSecure.cpp
// Description : Contains the implementation of the ObjectiveSecure c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "Objectives/ObjectiveSecure.h"
#include "Components/BoxComponent.h"

/**
 * @brief Programs the behaviour for the start of collision with this objective's "AreaToSecure" BoxComponent
 *		  It flips a boolean that allows for the time to decrease inside of the objective's Tick function
 * @param OverlappedComponent 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 * @param bFromSweep 
 * @param SweepResult 
 */
void AObjectiveSecure::BeginAreaOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == PlayerRef)
	{
		bPlayerInZone = true;
	}
}

/**
 * @brief Programs the behaviour for the end of collision with this objective's "AreaToSecure" BoxComponent
 *		  It flips a boolean to stop timing how long the player has been inside the space.
 * @param OverlappedComponent The box component this function is attached to
 * @param OtherActor The Actor that overlapped with the box.
 * @param OtherComp The component that overlapped with this box
 * @param OtherBodyIndex 
 */
void AObjectiveSecure::EndAreaOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor == PlayerRef)
	{
		bPlayerInZone = false;
	}
}

/**
 * @brief Sets default values
 */
AObjectiveSecure::AObjectiveSecure()
{
	// Creates the object, gives it an offset relative to the root component then sets its world rotation to 0 on all axes
	AreaToSecure = CreateDefaultSubobject<UBoxComponent>(TEXT("Area To Secure"));
	AreaToSecure->SetupAttachment(RootComponent);
	AreaToSecure->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
	AreaToSecure->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));

	// Set Movement of base component to a set location and to occur both in the PostEditMove and the PostEditUndo.
	// Do this instead of attaching the collision area as a child to the root.

	// Setting up the Collision Functions to work with AreaToSecure
	AreaToSecure->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveSecure::BeginAreaOverlap);
	AreaToSecure->OnComponentEndOverlap.AddDynamic(this, &AObjectiveSecure::EndAreaOverlap);
}

/**
 * @brief Called every frame
 * @param DeltaSeconds Time between frames
 */
void AObjectiveSecure::Tick(float DeltaSeconds)
{
	// Bypasses the parent Tick() and calls the AActor Tick() directly
	Super::Super::Tick(DeltaSeconds);

	// Check to see if the player has met the conditions for success on this objective
	// The conditions being that they've protected for long enough
	// And that they have not already succeeded.
	if (TimeSinceEntered >= SecondsRequired && !bPlayerSucceeded)
	{
		bPlayerSucceeded = true;

		// Trigger success conditions here
	}
	else if (bPlayerInZone && TimeSinceEntered < SecondsRequired)
	{
		TimeSinceEntered += DeltaSeconds;
	}
}
