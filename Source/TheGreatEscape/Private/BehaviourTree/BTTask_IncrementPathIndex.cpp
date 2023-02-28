// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_IncrementPathIndex.cpp
// Description : increment the path index
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_IncrementPathIndex.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyRework.h"
#include "BehaviourTree/BlackboardKeys.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller 
	//AEnemyReworkController* const AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	
	//if (AIController)
	//{
		//AEnemyRework* const Npc = Cast<AEnemyRework>(AIController->GetPawn());
		//int const NumOfPoints = Npc->GetPatrolPath()->Num(); // get number of points
		//int const MinIndex = 0;
		//int const MaxIndex = NumOfPoints - 1;

		// Get and set blackboard key
		//int index = AIController->GetBlackboard()->GetValueAsInt(BbKeys::patrolPathIndex);

		// Once NPC gets to end of points switch direction
		//if (index >= MaxIndex && Direction == EDirectionType::Forward)
		//{
			//Direction = EDirectionType::Backwards;
		//}
		//else if (index == MinIndex && Direction == EDirectionType::Backwards)
		//{
			//Direction = EDirectionType::Forward;
		//}

		// Increment / decrement patrol index based on direction of NPC
		//AIController->GetBlackboard()->SetValueAsInt(BbKeys::patrolPathIndex,(Direction == EDirectionType::Forward ? ++index : --index) % NumOfPoints);
		// Finish task
		//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//return EBTNodeResult::Succeeded;
	//}

	// Log warning that cast failed and finish task
	//UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	//FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
