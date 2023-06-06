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

#include "BehaviourTree/BTTask_PlayAudio.h"

#include "EnemyRework.h"
#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_PlayAudio::UBTTask_PlayAudio(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Play Audio");
}

EBTNodeResult::Type UBTTask_PlayAudio::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get controller & If ai controller not empty
	if (AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		
		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());
    	
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AlertSFX, Enemy->GetActorLocation() ,FRotator(0,0,0), 1.0f, 1.0f);

		Enemy->GetCharacterMovement()->MaxWalkSpeed = RunMoveSpeed;

		AIController->GetBlackboard()->SetValueAsBool(BbKeys::hasSeenPlayer, true);
		
		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
