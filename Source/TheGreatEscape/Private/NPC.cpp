// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : NPC.h
// Description : Npc character class
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "NPC.h"

#include "EnemyReworkController.h"

ANPC::ANPC()
{
	//npc
}

void ANPC::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEnemyReworkController* EnemyController = Cast<AEnemyReworkController>(NewController);

	if (EnemyController)
	{
		//EnemyController->EEnemyType = Utilities::EnemyTypes::Melee;
		EnemyController->SetBehaviourTree(Utilities::EnemyTypes::NPC);
		UE_LOG(LogTemp, Warning, TEXT("enemy type set NPC"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController NPC cast failed is: %s"), *NewController->GetName());
	}
	//
}

ANPCPatrolPath* ANPC::GetPatrolPath()
{
	return PatrolPath;
}
