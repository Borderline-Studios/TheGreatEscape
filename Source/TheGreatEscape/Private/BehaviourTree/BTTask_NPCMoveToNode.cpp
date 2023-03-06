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

UBTTask_NPCMoveToNode::UBTTask_NPCMoveToNode(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move To Node");
}

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
