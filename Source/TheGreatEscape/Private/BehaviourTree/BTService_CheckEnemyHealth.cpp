// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_CheckEnemyHealth.h
// Description : Checks whether train or player is closest to the target
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTService_CheckEnemyHealth.h"

#include "EnemyRework.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"

/**
 * @brief Constructor, defines the node's name
 */
UBTService_CheckEnemyHealth::UBTService_CheckEnemyHealth()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Check Enemy Health");
}

/**
 * @brief When the helper node becomes relevant it checks enemy health
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 */
void UBTService_CheckEnemyHealth::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	// get Ai controller & if AIController is not a nullptr
	if (const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get Enemy
		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());

		if (Enemy->CheckHealth() < HealthToCheck) // replace with value
		{
			AIController->GetBlackboard()->SetValueAsBool(BbKeys::enemyHealthLow, true);
		}
		
	}
}
