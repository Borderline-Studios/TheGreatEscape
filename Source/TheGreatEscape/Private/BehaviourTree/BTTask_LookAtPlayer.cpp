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
#include "EnemyReworkDrone.h"
#include "EnemyReworkHybrid.h"
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
 */
EBTNodeResult::Type UBTTask_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get controller & If ai controller not empty
	if (AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get player character
		if (ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			// Get players location
			FVector const PlayerLocation = Player->GetActorLocation();

			if (AEnemyReworkDrone* const Drone = Cast<AEnemyReworkDrone>(AIController->GetPawn()))
			{
				// get npc
				FVector Forward = PlayerLocation - Drone->GetActorLocation();
				FRotator Rot = UKismetMathLibrary::MakeRotFromXY(Forward, UKismetMathLibrary::Cross_VectorVector(Forward, FVector::DownVector));

				Drone->SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));
			
				FRotator newTurretBaseRot = UKismetMathLibrary::FindLookAtRotation(Drone->GetActorLocation(), PlayerLocation);

				Drone->TurretBaseRef->SetWorldRotation(newTurretBaseRot);

			}
			else if (AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn()))
			{
				// get npc
			

				FVector Forward = PlayerLocation - Enemy->GetActorLocation();
				FRotator Rot = UKismetMathLibrary::MakeRotFromXY(Forward, UKismetMathLibrary::Cross_VectorVector(Forward, FVector::DownVector));

				//FVector Rot = UKismetMathLibrary::FindLookAtRotation(NPC->GetActorLocation(), PlayerLocation);

				// set rotation of the actor
				Enemy->SetActorRotation(Rot);
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
