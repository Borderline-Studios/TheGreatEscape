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

/**
 * @brief Constructor, defines the node's name
 */
UBTService_TrainInRange::UBTService_TrainInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Drone In Attack Range");
}

/**
 * @brief When the helper node becomes relevant it checks if the player is within a specified range of the player 
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 */
void UBTService_TrainInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// call super
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// get Ai controller
	const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If AIController is not a nullptr
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
