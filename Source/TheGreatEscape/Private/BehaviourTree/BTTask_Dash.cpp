// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_Dash.h
// Description : Task for dasher to track player and start attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_Dash.h"

#include "EnemyRework.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_Dash::UBTTask_Dash(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Dash");
}

EBTNodeResult::Type UBTTask_Dash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get controller & If ai controller not empty
	if (AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get player character
		//ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());
    	
		// Get players location
		//FVector const PlayerLocation = Player->GetActorLocation();
		Enemy->GetMesh()->GetAnimInstance()->Montage_JumpToSection("DashAttack");
		Enemy->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UBTTask_Dash::Dash);

		if (bDashStart)
		{
			Enemy->GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, AIController->GetBlackboard()->GetValueAsVector(BbKeys::dashLocation));
			bDashStart = false;
		}


		if (FVector::Dist(Enemy->GetActorLocation(), AIController->GetBlackboard()->GetValueAsVector(BbKeys::dashLocation)) <= StoppingRange)
		{
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

void UBTTask_Dash::Dash(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("Attack"))
	{
		//AttackIndex = 0;
		bDashStart = true;
	}
}

