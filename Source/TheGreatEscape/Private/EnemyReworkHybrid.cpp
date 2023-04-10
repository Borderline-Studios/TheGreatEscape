// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkHybrid.cpp
// Description : Hybrid enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "EnemyReworkHybrid.h"
#include "EnemyReworkController.h"
#include "NiagaraFunctionLibrary.h"

/**
 * @brief Constructor
 */
AEnemyReworkHybrid::AEnemyReworkHybrid()
{
	// set can tick to true
	PrimaryActorTick.bCanEverTick = true;

	if (!DeathEffectReferenceHyrbid)
	{
		DeathEffectsHybrid = TSoftClassPtr<AEnemyRework>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Rework/BP_EnemyReworkHybridDeath.BP_EnemyReworkHybridDeath_C'")));
		DeathEffectReferenceHyrbid = DeathEffectsHybrid.LoadSynchronous();
	}
}

/**
 * @brief Sets up the tree using Drone enemy type
 * @param NewController Controller enemy is possessed by
 */
void AEnemyReworkHybrid::PossessedBy(AController* NewController)
{
	// call super
	Super::Super::PossessedBy(NewController);
	
	// Get enemy controller & if it is not nullptr
	if (AEnemyReworkController* EnemyController = Cast<AEnemyReworkController>(NewController))
	{
		// Set up the behaviour tree passing in enemy type Hybrid
		EnemyController->SetBehaviourTree(Utilities::EnemyTypes::Hybrid);
		//UE_LOG(LogTemp, Warning, TEXT("enemy type set drone"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController Hybrid Rework cast failed is: %s"), *NewController->GetName());
	}
}

/**
 * @brief Tick function
 * @param DeltaTime deltatime
 */
void AEnemyReworkHybrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyReworkHybrid::PostDeathProcess()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSFX, this->GetActorLocation(), this->GetActorRotation(), 0.5, FMath::RandRange(0.7, 1.4));
	// Spawn in body parts
	FVector location =  this->GetActorLocation();
	AActor* deathEffect = Cast<AActor>(GetWorld()->SpawnActor(DeathEffectReferenceHyrbid, &location));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, this->GetActorLocation(), this->GetActorRotation(), FVector(0.5,0.5,0.5), true);
}

void AEnemyReworkHybrid::PostHitProcess()
{
	Super::PostHitProcess();
	UE_LOG(LogTemp, Warning, TEXT("Hybrid hit"));
}

