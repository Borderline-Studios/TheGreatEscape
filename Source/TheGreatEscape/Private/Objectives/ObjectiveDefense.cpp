// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveDefense.cpp
// Description : Contains the implementations of the ObjectiveDefense class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "Objectives/ObjectiveDefense.h"
#include "Components/BoxComponent.h"

/**
 * @brief 
 * @param OverlappedComponent 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 * @param bFromSweep 
 * @param SweepResult 
 */
void AObjectiveDefense::BeginAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
 if (OtherActor == PlayerRef)
 {
  bPlayerInZone = true;
 }
}

/**
 * @brief 
 * @param OverlappedComponent 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 */
void AObjectiveDefense::EndAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
 if (OtherActor == PlayerRef)
 {
   bPlayerInZone = false;
 }
}

/**
 * @brief Default Constructor
 */
AObjectiveDefense::AObjectiveDefense()
{
     // Creates the object, gives it an offset relative to the root component then sets its world rotation to 0 on all axes
     AreaToDefend = CreateDefaultSubobject<UBoxComponent>(TEXT("Area To Secure"));
     AreaToDefend->SetupAttachment(RootComponent);
     AreaToDefend->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
     AreaToDefend->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));

     // Set Movement of base component to a set location and to occur both in the PostEditMove and the PostEditUndo.
     // Do this instead of attaching the collision area as a child to the root.

     // Setting up the Collision Functions to work with AreaToSecure
     AreaToDefend->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveDefense::BeginAreaOverlap);
     AreaToDefend->OnComponentEndOverlap.AddDynamic(this, &AObjectiveDefense::EndAreaOverlap);
}


void AObjectiveDefense::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

 // Bypasses the parent Tick() and calls the AActor Tick() directly
 Super::Super::Tick(DeltaSeconds);

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
