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
#include "EnemyReworkController.h"

/**
 * @brief Constructor
 */
AEnemyReworkDrone::AEnemyReworkDrone()
{
	// Child
}

/**
 * @brief Sets up the tree using Drone enemy type
 * @param NewController Controller enemy is possessed by
 */
void AEnemyReworkDrone::PossessedBy(AController* NewController)
{
	// call super
	Super::Super::PossessedBy(NewController);

	// Get enemy controller
	AEnemyReworkController* EnemyController = Cast<AEnemyReworkController>(NewController);

	// if it is not nullptr
	if (EnemyController)
	{
		// Set up the behaviour tree passing in enemy type drone
		EnemyController->SetBehaviourTree(Utilities::EnemyTypes::Drone);
		//UE_LOG(LogTemp, Warning, TEXT("enemy type set drone"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController Drone Rework cast failed is: %s"), *NewController->GetName());
	}
}
