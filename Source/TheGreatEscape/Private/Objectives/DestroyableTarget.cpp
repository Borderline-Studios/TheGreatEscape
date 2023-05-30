// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Objectives/DestroyableTarget.h"

// Sets default values
ADestroyableTarget::ADestroyableTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>("TargetMesh");
	RootComponent = TargetMesh;

}

// Called when the game starts or when spawned
void ADestroyableTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestroyableTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

