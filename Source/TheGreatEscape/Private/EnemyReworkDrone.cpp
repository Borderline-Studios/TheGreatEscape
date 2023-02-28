// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworKDrone.cpp
// Description : Drone enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "EnemyReworkDrone.h"

AEnemyReworkDrone::AEnemyReworkDrone()
{
	// Child
}

void AEnemyReworkDrone::PossessedBy(AController* NewController)
{
	Super::Super::PossessedBy(NewController);

	AEnemyReworkController* EnemyController = Cast<AEnemyReworkController>(NewController);

	if (EnemyController)
	{
		//EnemyController->EEnemyType = Utilities::EnemyTypes::Drone;
		EnemyController->SetBehaviourTree(Utilities::EnemyTypes::Drone);
		UE_LOG(LogTemp, Warning, TEXT("enemy type set drone"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController Drone Rework cast failed is: %s"), *NewController->GetName());
	}

	//Server Gas Init
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	
	AddStartupGameplayAbilities();
}
