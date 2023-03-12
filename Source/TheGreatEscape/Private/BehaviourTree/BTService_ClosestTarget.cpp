// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_CloestTarget.cpp
// Description : Checks whether train or player is closest to the target
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTService_ClosestTarget.h"

#include "EnemyReworkController.h"
#include "TrainEngine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"

/**
 * @brief Constructor, defines the node's name
 */
UBTService_ClosestTarget::UBTService_ClosestTarget()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Closest Target");
}

/**
 * @brief When the helper node becomes relevant it checks if the train or the player is closets
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 */
void UBTService_ClosestTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	// get Ai controller & if AIController is not a nullptr
	if (const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get Enemy
		APawn* const Enemy = AIController->GetPawn();

		// Get Train
		ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));

		// Get player character
		ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		
		// Set bool based on if train is closest (true) or if player closest (false)
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::isTrainClosest, (Enemy->GetDistanceTo(Train) + TrainCheckRange) <= Enemy->GetDistanceTo(Player));
		
	}
}
