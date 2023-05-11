// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   :
// Description :
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        : 

#include "Objectives/ObjectiveGate.h"

#include "TrainEngine.h"
#include "SplineTrack.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objectives/ObjectiveGateBatterySlot.h"
#include "Objectives/ObjectiveElevator.h"

/**
 * @brief Sets Default Values
 */
AObjectiveGate::AObjectiveGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	// Populating and setting up the Static mesh. This one gets set up as the root component,
	// Also setting the scale and the static mesh used by the mesh component
	GateFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Frame"));
	GateFrame->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> GateFrameMesh(TEXT("StaticMesh'/Game/Production/Enviroment/Interactables/Gate/Gate_GateFrame.Gate_GateFrame'"));
	GateFrame->SetStaticMesh(GateFrameMesh.Object);
	GateFrame->SetRelativeLocation(FVector(-450.0f, 0.0f, 0.0f));
	
	GateRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Right"));
	GateRight->SetupAttachment(GateFrame);
	ConstructorHelpers::FObjectFinder<UStaticMesh> GateRightMesh(TEXT("StaticMesh'/Game/Production/Enviroment/Interactables/Gate/Gate_GateLeft.Gate_GateLeft'"));
	GateRight->SetStaticMesh(GateRightMesh.Object);
	RightGateRelativeLocation = FVector(300.0f, 0.0f, 0.0f);
	GateRight->SetRelativeLocation(RightGateRelativeLocation);
	
	GateLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gate Left"));
	GateLeft->SetupAttachment(GateFrame);
	ConstructorHelpers::FObjectFinder<UStaticMesh> GateLeftMesh(TEXT("StaticMesh'/Game/Production/Enviroment/Interactables/Gate/Gate_GateRight.Gate_GateRight'"));
	GateLeft->SetStaticMesh(GateLeftMesh.Object);
	LeftGateRelativeLocation = FVector(-250.0f, 0.0f, 0.0f);
	GateLeft->SetRelativeLocation(LeftGateRelativeLocation);

	// Populating and setting up the Sphere Collision Component. Also sets the radius of the sphere.
	TrainDetector = CreateDefaultSubobject<USphereComponent>(TEXT("Train Detector"));
	TrainDetector->SetupAttachment(RootComponent);
	TrainDetector->SetSphereRadius(1000.0f);
	// TrainDetector->SetWorldLocation(GetActorLocation());
	// The lines below assign the BeginSphereOverlap to act as its OnComponentBeginOverlap function delegate.
	TrainDetector->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveGate::BeginTrainDetectorOverlap);
	
	// Populating the soft class pointer so that proper assets can be spawned.
	SlotClassRef = AObjectiveGateBatterySlot::StaticClass();
	PickupItemClassRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Objectives/PickUp/BP_PickUpObjective.BP_PickUpObjective_C'"))).LoadSynchronous();

	EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
}

/**
 * @brief Called when the game starts or when spawned
 */
void AObjectiveGate::BeginPlay()
{
	Super::BeginPlay();
	
	FixReferences();

	if (!EngineRef)
	{
		EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
	}
	
	SetActorTickEnabled(false);

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass()));
	}

	for (int i = 0; i < SlotRefs.Num(); i++)
	{
		Cast<AObjectiveGateBatterySlot>(SlotRefs[i])->InitialiseFromGate(this, PickupItemClassRef);
	}
}

/**
 * @brief Called every frame. Starts disabled for the actor, is switched on when the door is opened and then disabled again once the door has opened
 * @param DeltaTime The change in time between frames
 */
void AObjectiveGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceEnabled += DeltaTime;

	if (TimeSinceEnabled < GateSFX->GetDuration())
	{
		const float AlphaTime = TimeSinceEnabled / GateSFX->GetDuration();
		const FVector CurrentRightPos = FMath::Lerp(RightGateRelativeLocation, RightGateRelativeLocation + FVector(DoorMoveDistance, 0.0f, 0.0f), AlphaTime);
		const FVector CurrentLeftPos = FMath::Lerp(LeftGateRelativeLocation, LeftGateRelativeLocation + FVector(-DoorMoveDistance, 0.0f, 0.0f), AlphaTime);

		GateRight->SetRelativeLocation(CurrentRightPos);
		GateLeft->SetRelativeLocation(CurrentLeftPos);
	}
	else if (TimeSinceEnabled >= GateSFX->GetDuration())
	{
		bTrainStopped = false;
		EngineRef->EnableMovement();

		// Default call of this function sets the text back to ""
		UpdateObjectiveText();
		
		TimeSinceEnabled = 0;
		SetActorTickEnabled(false);
	}

}

void AObjectiveGate::UpdateObjectiveText(const FString NewText) const
{
	if (EngineRef)
	{
		EngineRef->UpdateObjectiveText(NewText);
	}
}

#if WITH_EDITOR	// This means that these functions only exist and apply while in the editor. These do not apply to packages
/**
 * @brief 
 */
void AObjectiveGate::Destroyed()
{
	Super::Destroyed();

	ClearPickups();
}

/**
 * @brief This Unreal function is called while an asset is being moved in editor.
 *		  The purpose of this one is to keep all of the separate components moving together as the SphereComponent isn't attached to the rootcomponent
 *		  It also makes the mesh move along the spline whe moving provided the required conditions are met
 * @param bFinished Whether or not the move action has completed. Allows for separation of functionality.
 */
void AObjectiveGate::PostEditMove(bool bFinished)
{
	// Calls the parent's version of this function. Was present during function generation
	Super::PostEditMove(bFinished);

	// Checks to see if the spline ref is populated and if we want to snap to the track.
	// If both are true (if the spline ref is populated and we want to snap to track) then the mesh will snap to the nearest point on the spline from where it's located in scene.
	if (SplineRef && bSnapToTrack)
	{
		Root->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}
}

/**
 * @brief This Unreal function is called whenever the undo operation is performed in editor on this item.
 *		  Currently sets the location of everything to where it was based on the previous location it occupied.
 */
void AObjectiveGate::PostEditUndo()
{
	Super::PostEditUndo();

	CleanPickupsArray();

	if (SplineRef && bSnapToTrack)
	{
		Root->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
	}
}

/**
 * @brief This custom function is only to exist in editor because that is the only place it will be used.
 *		  Sets the Location and Rotation of the spline to that of the point nearest it's location at the time of this function being called.
 *		  Only occurs if the conditions of the Spline reference being populated and the boolean being set to true in editor
 */
void AObjectiveGate::SnapRotation() const
{
	if (SplineRef && bSnapToTrack)
	{
		Root->SetWorldLocation(SplineRef->GetSpline()->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World));
		Root->SetWorldRotation(SplineRef->GetSpline()->FindRotationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World) + FRotator(0.0f, -90.0f, 0.0f));
	}
}

/**
 * @brief 
 */
void AObjectiveGate::SpawnPickup()
{
	FixReferences();
	
	AActor* NewSlot = GetWorld()->SpawnActor(SlotClassRef);
	NewSlot->SetActorLocation((GetActorLocation() + (GateFrame->GetRightVector() * (-250.00 - (200 * SlotRefs.Num())))) + FVector(0.0f, 0.0f, 100.0f));
	NewSlot->SetActorRotation(GetActorRotation());
	NewSlot->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	Cast<AObjectiveGateBatterySlot>(NewSlot)->SpawnPickup(PickupItemClassRef);
	
	SlotRefs.Push(NewSlot);
}

/**
 * @brief 
 */
void AObjectiveGate::RemovePickup()
{
	if (CleanPickupsArray()) return;		

	if (!SlotRefs.IsEmpty() && IsValid(SlotRefs.Last()))
	{
		SlotRefs.Last()->Destroy();
		SlotRefs.Pop();
	}
}

/**
 * @brief 
 */
void AObjectiveGate::ClearPickups()
{
	for (int i = 0; i < SlotRefs.Num(); i++)
	{
		if (SlotRefs[i])
		{
			SlotRefs[i]->Destroy();
		}
	}
	
	SlotRefs.Empty();
}
#endif

void AObjectiveGate::FixReferences()
{
	CleanPickupsArray();
	
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	
	for (int i = 0; i < AttachedActors.Num(); i++)
	{
		if (AObjectiveGateBatterySlot* CastActor = Cast<AObjectiveGateBatterySlot>(AttachedActors[i]))
		{
			SlotRefs.AddUnique(CastActor);
		}
	}
}

int AObjectiveGate::ElevatorInformationCheck(int RequirementToAdjust)
{
	FixReferences();

	const bool bValueInRange = UKismetMathLibrary::InRange_IntInt(RequirementToAdjust, 0, SlotRefs.Num(), true, false);

	if (!bValueInRange && SlotRefs.Num() >= 2)
	{
		RequirementToAdjust = SlotRefs.Num() - 1;
	}
	else
	{
		RequirementToAdjust = 0;
	}

	return RequirementToAdjust;
}

bool AObjectiveGate::CleanPickupsArray()
{
	bool bCleanedUp = false;
	
	for (int i = SlotRefs.Num() - 1; i >= 0; i--)
	{
		if (!IsValid(SlotRefs[i]))
		{
			SlotRefs.RemoveAt(i);

			if (!bCleanedUp)
			{
				bCleanedUp = true;
			}
		}
	}

	return bCleanedUp;
}

/**
 * @brief Default Unreal function that allows for the handling of collisions for specific objects. The specific objects are set separately, usually inside the constructor.
 *		  This instance handles two collision instances:
 *		  - The event that the train is colliding with the Train Detector Sphere Component.
 *			-> Sets the train to stop and sets the internal tracking boolean of bTrainStopped to true so that it doesn't repeat.
 *		  - The event that the PickupItem is brought into the sphere.
 *			-> If the Train has been stopped, it allows for the train to start back up again.
 * @param OverlappedComponent 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 * @param bFromSweep 
 * @param SweepResult 
 */
void AObjectiveGate::BeginTrainDetectorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!bOpened)
	{
		// If the object colliding with the sphere is the train engine AND the internal tracking variable says we haven't stopped it yet...
		// Stop the Train and update the tracking boolean
		if (OtherActor == EngineRef && OtherComp == EngineRef->GetEngineDetectionComponent() && !bTrainStopped)
		{
			bTrainStopped = true;
			EngineRef->ToggleTrainStop();
			EngineRef->DisableMovement();
			FString ObjText = "Collect ";
			ObjText.AppendInt(SlotRefs.Num());
			ObjText.Append(((SlotRefs.Num() == 1) ? " Battery." : " Batteries."));
			ObjText.Append(" Explore nearby!");
			UpdateObjectiveText(ObjText);
		}
	}
}

void AObjectiveGate::UpdateFromSlot()
{
	if (!bOpened)
	{
		int SlotsFilled = 0;
		
		for (int i = 0; i < SlotRefs.Num(); i++)
		{
			if (Cast<AObjectiveGateBatterySlot>(SlotRefs[i])->GetSlotFilled())
			{
				SlotsFilled++;
			}
		}

		if (ElevatorRef)
		{
			ElevatorRef->EnableElevator(SlotsFilled);
		}

		if (SlotsFilled != (SlotRefs.Num()))
		{
			return;
		}

		bOpened = true;
		SetActorTickEnabled(true);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GateSFX, GetActorLocation(), FRotator(), 1.0f);
	}
}
