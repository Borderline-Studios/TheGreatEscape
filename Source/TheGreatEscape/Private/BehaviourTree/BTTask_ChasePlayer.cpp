// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_ChasePlayer.h
// Description : Chases after the player
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_ChasePlayer.h"
#include "EnemyReworkController.h"
#include "TrainEngine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviourTree/Utils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Chase Target");
}

/**
 * @brief When node is executed it chases the player 
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Ai controller
	AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If ai controller not empty
	if (AIController)
	{
		
		// Get targets location
		FVector const TargetLocation = AIController->GetBlackboard()->GetValueAsVector(BbKeys::targetLocation);

		// Get Enemy
		APawn* const Enemy = AIController->GetPawn();

		FVector distance = TargetLocation - Enemy->GetActorLocation();

		// Get player character
		ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
            	
		// Get players location
		FVector const PlayerLocation = Player->GetActorLocation();


		// get train engine
		ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
		
		if (distance.Dist(TargetLocation, Enemy->GetActorLocation()) >= StoppingDistance)
		{
			if (bChasePlayer)
			{
				// move to player location
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, PlayerLocation);
			}
			else
			{
				// move to player location
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, Train->GetActorLocation());
			}
			
		}
		else
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, Enemy->GetActorLocation());
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
