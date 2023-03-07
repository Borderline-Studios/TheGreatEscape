// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FollowTrain.cpp
// Description : Follows the train
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_FollowTrain.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TrainEngine.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_FollowTrain::UBTTask_FollowTrain(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Follow Train");
}

/**
 * @brief When node is executed it follows the train
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_FollowTrain::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get controller
	AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If ai controller not empty
	if (AIController)
	{
		// get train engine
		ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));

		// get enemy
		APawn* const Enemy = AIController->GetPawn();

		// if train not nullptr
		if (Train)
		{
			// locations
			FVector TrainLocation = Train->GetActorLocation();
			FVector EnemyLocation = Enemy->GetActorLocation();

			// find direction
			FVector direction = FVector(TrainLocation.X, TrainLocation.Y, TrainLocation.Z + EvelvationHeight) - EnemyLocation;

			// multiple direction by speed
			direction *= Speed;

			// set velocity of enemy
			Enemy->GetMovementComponent()->Velocity = direction;
	
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cast to ATrainEngine failed"));
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
