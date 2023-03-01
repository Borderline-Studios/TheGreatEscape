// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_TrainInRange.h
// Description : Checks enemy can attack the train
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz



#include "BehaviourTree/BTService_TrainInRange.h"

#include "EnemyReworkController.h"
#include "TrainEngine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_TrainInRange::UBTService_TrainInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Drone In Attack Range");
}

void UBTService_TrainInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// get Ai controller and enemy
	const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	
	if (AIController)
	{
		// Get Enemy
		APawn* const Enemy = AIController->GetPawn();

		// Get Train
		ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
		

		// Set bool based on if player is in melee range by finding distance between them
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::droneInAttackRange, Enemy->GetDistanceTo(Train) <= AttackRange);
	}
}
