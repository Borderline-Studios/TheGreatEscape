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


#include "BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyReworkController.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BlackboardKeys.h"
//#include "EnemyReworkController.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller and enemy
	auto const AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		auto const Enemy = AIController->GetPawn();

		// Get enemy location to use as starting point
		FVector const Origin = Enemy->GetActorLocation();
		FNavLocation Location;

		// Get nav system and generate random location on NavMesh
		UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem->GetRandomReachablePointInRadius(Origin, SearchRadius, Location, nullptr))
		{
			AIController->GetBlackboard()->SetValueAsVector(BbKeys::randLocation, Location.Location);
		}

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
