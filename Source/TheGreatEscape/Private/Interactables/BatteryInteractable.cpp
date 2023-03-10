// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/BatteryInteractable.h"
#include "Components/CapsuleComponent.h"




// Sets default values
ABatteryInteractable::ABatteryInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// CapsuleComponent initialization
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(
											 TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(44.f, 60.f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh1P"));
	StaticMesh->SetupAttachment(CapsuleComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// Direct reference to static mesh object from content browser ^^^
	StaticMesh->SetStaticMesh(MeshObj.Object);

}

// Called when the game starts or when spawned
void ABatteryInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABatteryInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABatteryInteractable::SetPickedUp()
{
	SetActorEnableCollision(false);
	IsPickedUp = true;
}

