// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FindPlayerLocation.cpp
// Description : Finds player location
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TheGreatEscape/TheGreatEscapeCharacter.h"
#include "BehaviourTree/BlackboardKeys.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get player character & Enemy AI controller
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	const AEnemyReworkController*  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// Check AIController not null
	if (AIController)
	{
		// Get player location
		FVector const PlayerLocation = player->GetActorLocation();

		// If searching for random point
		//if (bSearchRandom)
		//{
			//FNavLocation Location;

			// Get nav system and generate random location near the player
			//UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
			//if (NavSystem->GetRandomReachablePointInRadius(PlayerLocation, SearchRadius, Location, nullptr))
			//{
				//AIController->GetBlackboard()->SetValueAsVector(BbKeys::targetLocation, Location.Location);
			//}
		//}
		//else // set location as player location
		//{
			AIController->GetBlackboard()->SetValueAsVector(BbKeys::targetLocation, PlayerLocation);
		//}

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
		return EBTNodeResult::Succeeded;
	}
	
	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	return EBTNodeResult::Failed;
}
