// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyAIController.cpp
// Description : 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz



#include "EnemyAIController.h"
#include "NavigationSystem.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	//RandomPatrol();
}

void AEnemyAIController::RandomPatrol()
{
	if (NavArea)
	{
		if (GetPawn())
		{
			NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(),RandomLocation, 15000.0f);

			MoveToLocation(RandomLocation);

			UE_LOG(LogTemp, Warning, TEXT("Random Location %s"), *RandomLocation.ToString());
		}
	}
}
