// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FindPatrolPathPoint.cpp
// Description : Find the point in the vector to go to
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_FindPatrolPathPoint.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC.h"
#include "BehaviourTree/Utils.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_FindPatrolPathPoint::UBTTask_FindPatrolPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Patrol Path Point");
}

/**
 * @brief When node is executed it finds the next patrol point 
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_FindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller  & if not nullptr
	if (AEnemyReworkController* const AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get current index from blackboard
		int const Index = AIController->GetBlackboard()->GetValueAsInt(BbKeys::patrolPathIndex);

		// use index to get the current patrol path from npc ref to patrol path
		ANPC* const Npc = Cast<ANPC>(AIController->GetPawn());
		FVector const Point = Npc->GetPatrolPath()->GetPatrolPoint(Index);

		// Transform local point to global point using parent
		FVector const GlobalPoint = Npc->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

		// write current global path point to blackboard
		AIController->GetBlackboard()->SetValueAsVector(BbKeys::patrolPathVector, GlobalPoint);

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
