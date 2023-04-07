// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_NPCMoveToNode.h
// Description : Moves the npc to the node
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_StopHearing.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviourTree/Utils.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_StopHearing::UBTTask_StopHearing(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Stop Hearing");
}

/**
 * @brief When node is executed it changes bool of enemy that says it can hear sound
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_StopHearing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// Get AI controller & if not nullptr
	if (const AEnemyReworkController*  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// set the bool back to false
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::hasHeardNoise, false);

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task stop hearing failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
