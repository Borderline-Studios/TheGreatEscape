// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_ResetDash.h
// Description : The dash attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_ResetDash.h"

#include "EnemyRework.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_ResetDash::UBTTask_ResetDash(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Reset Dash");
}

EBTNodeResult::Type UBTTask_ResetDash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get controller & If ai controller not empty
	if (AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());
		
		Enemy->GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed;
		AIController->StopMovement();
		Enemy->GetMesh()->GetAnimInstance()->Montage_JumpToSection("DashEnd");
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::isDashing, false);
		
		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
