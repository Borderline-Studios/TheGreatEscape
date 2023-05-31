// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_DasherTrackPlayer.h
// Description : Task for dasher to track player and start attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_DasherTrackPlayer.h"

#include "EnemyRework.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_DasherTrackPlayer::UBTTask_DasherTrackPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Track Player");
}

EBTNodeResult::Type UBTTask_DasherTrackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get controller & If ai controller not empty
	if (AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get player character
		ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());
    	
		// Get players location
		//FVector const PlayerLocation = Player->GetActorLocation();

		if (bCanDash)
		{
			Enemy->GetMesh()->GetAnimInstance()->Montage_JumpToSection("DashWindUp");
			GetWorld()->GetTimerManager().SetTimer(DashDelayHandle, this, &UBTTask_DasherTrackPlayer::SetCanDash, TrackingTime, false);
			bCanDash = false;
		}

		if (bDashing)
		{
			//Enemy->GetMesh()->GetAnimInstance()->Montage_JumpToSection("DashAttack");
			AIController->GetBlackboard()->SetValueAsBool(BbKeys::isDashing, true);
			AIController->GetBlackboard()->SetValueAsVector(BbKeys::dashLocation, Player->GetActorLocation());
			bDashing = false;
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

void UBTTask_DasherTrackPlayer::SetCanDash()
{

	// can attack
	bCanDash = true;

	bDashing = true;

	// clear timer
	GetWorld()->GetTimerManager().ClearTimer(DashDelayHandle);
}
