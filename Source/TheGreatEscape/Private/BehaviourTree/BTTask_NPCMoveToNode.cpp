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


#include "BehaviourTree/BTTask_NPCMoveToNode.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_NPCMoveToNode::UBTTask_NPCMoveToNode(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move To Node");
}

/**
 * @brief When the helper node becomes relevant it moves the npc to target node
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_NPCMoveToNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If ai controller not empty
	if (AIController)
	{
		// Get npcs next node location
		FVector const NextNode = AIController->GetBlackboard()->GetValueAsVector(BbKeys::patrolPathVector);

		// get enemy
		APawn* const NPC = AIController->GetPawn();

		// if dist is within acceptable radius, change can increment to true
		if (FVector::Dist(NPC->GetActorLocation(), NextNode) <= AcceptanceRadius)
		{
			AIController->GetBlackboard()->SetValueAsBool(BbKeys::NPCCanIncrement, true);
		}
		else
		{
			AIController->GetBlackboard()->SetValueAsBool(BbKeys::NPCCanIncrement, false);
		}
		
		// move to node location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, NextNode);

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
