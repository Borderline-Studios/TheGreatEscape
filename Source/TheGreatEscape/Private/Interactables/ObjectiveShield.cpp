// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Interactables/ObjectiveShield.h"

// Sets default values
AObjectiveShield::AObjectiveShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectiveShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectiveShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObjectiveShield::PostHitProcess()
{
	ShieldHealth = ShieldHealth - DamageToBreak;
	if (ShieldHealth <= 0)
	{
		PostDestroyProcess();
	}
}

void AObjectiveShield::PostDestroyProcess()
{
	Destroy();
}

