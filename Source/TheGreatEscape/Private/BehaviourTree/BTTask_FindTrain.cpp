// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FindTrain.cpp
// Description : Finds Train location
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_FindTrain.h"

#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviourTree/BlackboardKeys.h"
#include "TrainEngine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BehaviourTree/BlackboardKeys.h"


UBTTask_FindTrain::UBTTask_FindTrain(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Train Location");
}

EBTNodeResult::Type UBTTask_FindTrain::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get AI controller
	const AEnemyReworkController*  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		// Get train
		ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));

		if (Train)
		{
			FVector const TrainLocation = Train->GetActorLocation();

			AIController->GetBlackboard()->SetValueAsVector(BbKeys::targetLocation, TrainLocation);
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
