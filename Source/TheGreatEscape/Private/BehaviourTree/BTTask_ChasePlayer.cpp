// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_ChasePlayer.h
// Description : Chases after the player
// Author      :  Borderline Studios - Toni Natta
// Mail        :  toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_ChasePlayer.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviourTree/BlackboardKeys.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If ai controller not empty
	if (AIController)
	{
		// Get players location
		FVector const PlayerLocation = AIController->GetBlackboard()->GetValueAsVector(BbKeys::targetLocation);

		// move to player location
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, PlayerLocation);

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	return EBTNodeResult::Failed;
}
