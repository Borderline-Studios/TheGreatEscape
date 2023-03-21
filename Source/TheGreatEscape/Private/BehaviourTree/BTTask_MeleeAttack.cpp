// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_MeleeAttack.cpp
// Description : Melee attack the player
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_MeleeAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnemyReworkController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Character/BASE/GASBASECharacter.h"
#include "Character/Player/PlayerCharacter.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_MeleeAttack::UBTTask_MeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
}

/**
 * @brief When the helper node becomes relevant it attacks the player
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller
	const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// if AI controller is not nullptr
	if (AIController)
	{
		// Get enemy
		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());
		
		// if enemy is not null
		if (Enemy)
		{
			// if can attack
			if (bCanAttack)
			{
				Enemy->bAttacking = true;
				// Get player character
				APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

				// if it is the player
				if (PlayerChar)
				{
					//UE_LOG(LogTemp, Warning, TEXT("atac"));

					Enemy->GetMesh()->GetAnimInstance()->Montage_JumpToSection("Attack");
					
					// call attack
					Enemy->GetAbilitySystemComponent()->TryActivateAbilityByClass(Enemy->QRGAAttack, true);

					// set bool to false and set timer
					bCanAttack = false;
					GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &UBTTask_MeleeAttack::SetCanAttack, AttackDelay, false);
				}
			}
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

/**
 * @brief Sets the attack bool to true and clears the timer
 */
void UBTTask_MeleeAttack::SetCanAttack()
{
	// can attack
	bCanAttack = true;

	// clear timer
	GetWorld()->GetTimerManager().ClearTimer(AttackDelayHandle);
}
