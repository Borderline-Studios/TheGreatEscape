// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_NPCInRange.h
// Description : Checks if npc in player range
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTService_NPCInRange.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviourTree/Utils.h"
#include "Runtime/Engine/Classes/Engine/World.h"

/**
 * @brief Constructor, defines the node's name
 */
UBTService_NPCInRange::UBTService_NPCInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is NPC In Range");
}

/**
 * @brief When the helper node becomes relevant it checks if the player is within a specified range of the player 
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 */
void UBTService_NPCInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Call super
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	// Get Ai controller
	const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If AIController is not a nullptr
	if (AIController)
	{
		// Get NPC from the ai controller
		APawn* const NPC = AIController->GetPawn();

		// Get player character
		ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		// Set bool based on if player is in melee range by finding distance between them
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::NPCInPlayerRange, NPC->GetDistanceTo(Player) <= PlayerRange);
	}
}
