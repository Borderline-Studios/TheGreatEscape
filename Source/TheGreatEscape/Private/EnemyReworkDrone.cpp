// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyReworkDrone.h"

AEnemyReworkDrone::AEnemyReworkDrone()
{
	// Child
}

void AEnemyReworkDrone::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEnemyReworkController* EnemyController = Cast<AEnemyReworkController>(NewController);

	if (EnemyController)
	{
		EnemyController->EEnemyType = Utilities::EnemyTypes::Melee;
		UE_LOG(LogTemp, Warning, TEXT("enemy type set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController cast failed"));
	}
}
