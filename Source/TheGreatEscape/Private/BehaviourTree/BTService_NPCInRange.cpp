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
#include "BehaviourTree/BlackboardKeys.h"
#include "Runtime/Engine/Classes/Engine/World.h"

UBTService_NPCInRange::UBTService_NPCInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is NPC In Range");
}

void UBTService_NPCInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	
	// get Ai controller and enemy
	const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	
	if (AIController)
	{
		// Get Enemy
		APawn* const NPC = AIController->GetPawn();

		// Get player character
		ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		// Set bool based on if player is in melee range by finding distance between them
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::NPCInPlayerRange, NPC->GetDistanceTo(Player) <= PlayerRange);

		if (NPC->GetDistanceTo(Player) <= PlayerRange)
		{
			UE_LOG(LogTemp, Warning, TEXT("is in range"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("no"));
		}
		
	}
}
