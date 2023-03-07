// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FindRandomLocation.cpp
// Description : Finds random location for ai to go to
// Author      :  Borderline Studios - Toni Natta
// Mail        :  toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyReworkController.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviourTree/BlackboardKeys.h"
// #include "SNegativeActionButton.h"
//#include "EnemyReworkController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller and enemy
	const AEnemyReworkController*  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	
	if (AIController)
	{
		APawn* const Enemy = AIController->GetPawn();

		// Get enemy location to use as starting point
		FVector const Origin = Enemy->GetActorLocation();
		FNavLocation Location;

		// Get nav system and generate random location on NavMesh
		UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem->GetRandomReachablePointInRadius(Origin, SearchRadius, Location, nullptr))
		{
			AIController->GetBlackboard()->SetValueAsVector(BbKeys::targetLocation, Location.Location);
		}

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
		return EBTNodeResult::Succeeded;
	}

	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	return EBTNodeResult::Failed;
}
