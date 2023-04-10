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
#include "TrainEngine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TheGreatEscape/TheGreatEscapeCharacter.h"
#include "BehaviourTree/Utils.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Actor Location");
}

/**
 * @brief When node is executed it finds a random location based on players location
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get player character & Enemy AI controller
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	const AEnemyReworkController*  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	if (!bStationaryHybrid)
	{
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::stationaryHybrid, false);
	}
	else
    {
    	AIController->GetBlackboard()->SetValueAsBool(BbKeys::stationaryHybrid, true);
    }

	// Check AIController not null
	if (AIController)
	{
		if (bSearchForPlayer)
		{
			// Get player location
			FVector const PlayerLocation = player->GetActorLocation();

			// Update bb key
			AIController->GetBlackboard()->SetValueAsVector(BbKeys::targetLocation, PlayerLocation);
		}
		else
		{
			// Get Train
			ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
			
			// Update bb key
			AIController->GetBlackboard()->SetValueAsVector(BbKeys::targetLocation, Train->GetActorLocation());
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
