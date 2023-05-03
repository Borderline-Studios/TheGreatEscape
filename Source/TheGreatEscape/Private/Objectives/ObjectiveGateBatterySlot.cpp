// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Objectives/ObjectiveGateBatterySlot.h"
#include "Objectives/ObjectiveGate.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObjectiveGateBatterySlot::AObjectiveGateBatterySlot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SlotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Slot Mesh"));
	RootComponent = SlotMesh;
    const ConstructorHelpers::FObjectFinder<UStaticMesh> SlotStaticMesh(TEXT("StaticMesh'/Game/Production/Enviroment/Interactables/Gate/S_BatterySlot.S_BatterySlot'"));
	SlotMesh->SetStaticMesh(SlotStaticMesh.Object);

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Battery Detector"));
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->SetSphereRadius(400.0f);
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveGateBatterySlot::BeginBatteryDetectorOverlap);
}

// Called when the game starts or when spawned
void AObjectiveGateBatterySlot::BeginPlay()
{
	Super::BeginPlay();

	if (!PlayerRef)
	{
		PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass()));
	}
}

void AObjectiveGateBatterySlot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (PlayerRef) PlayerRef = nullptr;
}

// Called every frame
void AObjectiveGateBatterySlot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AObjectiveGateBatterySlot::BeginBatteryDetectorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Check to see if other actor is player
	if (OtherActor == PlayerRef)
	{
		// if so, check to see if it's holding a battery AND that we're empty
		if (!bSlotFilled && PlayerRef->bBatteryPickedUp)
		{
			// if true
			// make it stop holding the battery
			PlayerRef->bBatteryPickedUp = false;
			
			// mark ourselves as being filled
			bSlotFilled = true;
			
			// spawn battery at filled location
			AActor* PlacedBattery = GetWorld()->SpawnActor(PickupItemClassRef);
			PlacedBattery->AttachToComponent(DetectionSphere, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PlacedBattery->SetActorRelativeLocation(FVector(30.0f, 5.0f, 160.0f));
			PlacedBattery->SetActorEnableCollision(false);
			PlacedBattery->Tags.Empty();
			
			// update objective text? -> call a function inside the gate that only this class has access to
			GateRef->UpdateFromSlot();
		}
	}
}


void AObjectiveGateBatterySlot::Destroyed()
{
	Super::Destroyed();

	if (PickupItem) PickupItem->Destroy();
}

/**
 * @brief Public function that allows external classes to know whether the slot ahs been filled or not
 * @return Whether or not the slot has been filled by a battery
 */
bool AObjectiveGateBatterySlot::GetSlotFilled()
{
	return bSlotFilled;
}

/**
 * @brief Spawns the pickup, called exclusively from the ObjectiveGate Class
 * @param NewPickupItemClass The class of the pickup to be spawned
 */
void AObjectiveGateBatterySlot::SpawnPickup(UClass* NewPickupItemClass)
{
	PickupItem = GetWorld()->SpawnActor(NewPickupItemClass);
	PickupItem->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 150.0 + FVector(0.0f, 0.0f, 100.0f));
	PickupItem->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

/**
 * @brief Populates the gate reference that this slot will be referring to.
 * Important because it determines which gate is updated when the slot gets filled
 * @param NewGateRef The gate this item is attached to
 * @param NewPickupItemClass Class that the pickup item will be spawned as
 */
void AObjectiveGateBatterySlot::InitialiseFromGate(AObjectiveGate* NewGateRef, UClass* NewPickupItemClass)
{
	GateRef = NewGateRef;
	PickupItemClassRef = NewPickupItemClass;
}