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
#include "Character/QRCharacter.h"



UBTTask_MeleeAttack::UBTTask_MeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller and enemy
	const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	
	if (AIController)
	{

		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());
		// Get Enemy
		if (Enemy)
		{
			if (bCanAttack)
			{
				// Get player character & Enemy AI controller
				//ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

				AQRCharacter* PlayerChar = Cast<AQRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
				
				if (PlayerChar)
				{
					UE_LOG(LogTemp, Warning, TEXT("atac"));
					
					// call attack
					Enemy->GetAbilitySystemComponent()->TryActivateAbilityByClass(Enemy->QRGAAttack, true);
					
					bCanAttack = false;
					GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &UBTTask_MeleeAttack::SetCanAttack, AttackDelay, false);
					//UE_LOG(LogTemp, Warning, TEXT("Cast to ACharacter is: %s"), *PlayerChar->GetName())

				}
			}
			
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyRework failed it is: %s"), *Enemy->GetName());
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

void UBTTask_MeleeAttack::SetCanAttack()
{
	// can attack
	bCanAttack = true;

	// clear timer
	GetWorld()->GetTimerManager().ClearTimer(AttackDelayHandle);
}
