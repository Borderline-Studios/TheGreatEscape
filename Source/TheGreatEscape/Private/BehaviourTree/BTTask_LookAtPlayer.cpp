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
 */
EBTNodeResult::Type UBTTask_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get controller
	AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If ai controller not empty
	if (AIController)
	{
		// Get players location
		FVector const PlayerLocation = AIController->GetBlackboard()->GetValueAsVector(BbKeys::targetLocation);

		// get npc
		ANPC* const NPC = Cast<ANPC>(AIController->GetPawn());

		// set original rotation
		if (!bOriginalRotSet)
		{
			OriginalRotation = NPC->GetActorRotation();
			bOriginalRotSet = true;
		}

		// look at player
		FVector LookDir = PlayerLocation - NPC->GetActorLocation();
		LookDir.Normalize();

		FRotator LookAt = FRotationMatrix::MakeFromX(LookDir).Rotator();
		LookAt.Pitch = 0.0f;
		LookAt.Roll = 0.0f;
		
		if (OriginalRotation != LookAt)
		{
			FRotator newRotation = FMath::RInterpTo(OriginalRotation, LookAt, GetWorld()->GetDeltaSeconds(), RotationSpeed);
			NPC->SetActorRotation(newRotation);
		}
		else
		{
			bOriginalRotSet = false;
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
