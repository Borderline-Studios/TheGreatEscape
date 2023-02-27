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


#include "BTTask_FindPatrolPathPoint.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyRework.h"
#include "BlackboardKeys.h"

UBTTask_FindPatrolPathPoint::UBTTask_FindPatrolPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Patrol Path Point");
}

EBTNodeResult::Type UBTTask_FindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller 
	AEnemyReworkController* const AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	
	if (AIController)
	{
		// Get current index from blackboard
		int const Index = AIController->GetBlackboard()->GetValueAsInt(BbKeys::patrolPathIndex);

		// use index to get the current patrol path from npc ref to patrol path
		AEnemyRework* const Npc = Cast<AEnemyRework>(AIController->GetPawn());
		//FVector const Point = Npc->GetPatrolPath()->GetPatrolPoint(Index);

		// Transform local point to global point using parent
		//FVector const GlobalPoint = Npc->GetPatrolPath()->GetActorTransform().TransformPosition(Point);

		// write current global path point to blackboard
		//AIController->GetBlackboard()->SetValueAsVector(BbKeys::patrolPathVector, GlobalPoint);

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
