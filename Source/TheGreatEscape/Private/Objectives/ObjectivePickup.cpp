// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Objectives/ObjectivePickup.h"

#include "Components/BoxComponent.h"

void AObjectivePickup::BeginAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

AObjectivePickup::AObjectivePickup()
{
	// Creates the object, gives it an offset relative to the root component then sets its world rotation to 0 on all axes
	CollectionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("Collection Zone"));
	CollectionZone->SetupAttachment(RootComponent);
	CollectionZone->SetRelativeLocation(FVector(0.0f, 100.0f, 0.0f));
	CollectionZone->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));

	// Set Movement of base component to a set location and to occur both in the PostEditMove and the PostEditUndo.
	// Do this instead of attaching the collision area as a child to the root.

	// Setting up the Collision Functions to work with CollectionZone
	CollectionZone->OnComponentBeginOverlap.AddDynamic(this, &AObjectivePickup::BeginAreaOverlap);

	// PICKUP OBJECT -> Doesn't Work! 
	ObjectToPickup = CreateDefaultSubobject<AActor>(TEXT("TEST"));
	ObjectToPickup->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	// ObjectToPickup->SetActorLocation(GetActorLocation() + FVector(0.0f, -100.0f, 0.0f));
	UStaticMeshComponent* ObjStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	ObjStaticMeshComp->AttachToComponent(ObjectToPickup->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/Production/Interactables/Placeables/Pickups/Battery/Battery.Battery'"));
	ObjStaticMeshComp->SetStaticMesh(MeshObj.Object);
	ObjectToPickup->Tags.Push("Interactable");
}

void AObjectivePickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
