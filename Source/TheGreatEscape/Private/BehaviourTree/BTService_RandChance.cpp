// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_RandChance.h
// Description : Has a random chance to return true
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTService_RandChance.h"

#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"

/**
 * @brief Constructor, defines the node's name
 */
UBTService_RandChance::UBTService_RandChance()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Random Chance");
}

/**
 * @brief When the helper node becomes relevant it checks enemy health
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 */
void UBTService_RandChance::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	// get Ai controller & if AIController is not a nullptr
	if (const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		float randomNum = FMath::RandRange(1.0f, Chance);

		if (randomNum == 1)
		{
			AIController->GetBlackboard()->SetValueAsBool(BbKeys::randChance, true);
			UE_LOG(LogTemp, Warning, TEXT("TrrUREEE"));
		}
		else
		{
			AIController->GetBlackboard()->SetValueAsBool(BbKeys::randChance, false);
		}
		
	}
}
