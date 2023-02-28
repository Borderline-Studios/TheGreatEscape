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
#include "BehaviourTree/BlackboardKeys.h"
#include "TrainEngine.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FollowTrain::UBTTask_FollowTrain(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Follow Train");
}

EBTNodeResult::Type UBTTask_FollowTrain::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If ai controller not empty
	if (AIController)
	{
		ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));

		if (Train)
		{
			FVector const TrainLocation = Train->GetActorLocation();
			
			// move to player location
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TrainLocation);

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
