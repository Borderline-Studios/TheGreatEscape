// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_LookAtPlayer.cpp
// Description : Task to look at the player
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "BehaviourTree/BTTask_LookAtPlayer.h"

#include "EnemyReworkController.h"
#include "NPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_LookAtPlayer::UBTTask_LookAtPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Look At Player");
}

/**
 * @brief When node is executed it makes npc face the player
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 * @bug Doesnt rotate properly, needs fix 
 */
EBTNodeResult::Type UBTTask_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get controller & If ai controller not empty
	if (AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get players location
		FVector const PlayerLocation = AIController->GetBlackboard()->GetValueAsVector(BbKeys::targetLocation);

		// get npc
		ANPC* const NPC = Cast<ANPC>(AIController->GetPawn());

		FVector Forward = PlayerLocation - NPC->GetActorLocation();
		FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(Forward, FVector::UpVector);
		NPC->SetActorRotation(Rot);
		//NPC->SetActorRotation(newRot);

		// Finish task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// Log warning that cast failed and finish task
	UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}
