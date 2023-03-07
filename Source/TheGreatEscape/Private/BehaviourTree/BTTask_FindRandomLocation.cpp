// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FindRandomLocation.cpp
// Description : Finds random location for ai to go to
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyReworkController.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviourTree/BlackboardKeys.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location");
}

/**
 * @brief When node is executed it finds a random location near the player
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller and enemy
	const AEnemyReworkController*  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (AIController)
	{
		APawn* const Enemy = AIController->GetPawn();

		// Get player location
		FVector const PlayerLocation = player->GetActorLocation();

		// FLocation for enemy to go
		FNavLocation Location;

		// Get nav system and generate random location near the player
		UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSystem->GetRandomReachablePointInRadius(PlayerLocation, SearchRadius, Location, nullptr))
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
