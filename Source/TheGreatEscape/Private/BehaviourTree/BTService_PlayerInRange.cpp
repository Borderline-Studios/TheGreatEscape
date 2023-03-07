// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_PlayerInRange.h
// Description : Checks in player is in melee range
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTService_PlayerInRange.h"
#include "EnemyRework.h"
#include "EnemyReworkController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviourTree/BlackboardKeys.h"

/**
 * @brief Constructor, defines the node's name
 */
UBTService_PlayerInRange::UBTService_PlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Range");
}

/**
 * @brief When the helper node becomes relevant it checks if the player is within a specified range of the player 
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 */
void UBTService_PlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		// Get player character
		ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		// Set bool based on if player is in melee range by finding distance between them
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::playerInMeleeRange, Enemy->GetDistanceTo(Player) <= MeleeRange);
	}
}
