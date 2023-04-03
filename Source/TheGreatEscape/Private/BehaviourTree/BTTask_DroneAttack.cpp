// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_DroneAttack.cpp
// Description : Attack functionality for the drone
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_DroneAttack.h"

#include "EnemyReworkDrone.h"
#include "EnemyReworkController.h"
#include "TrainEngine.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_DroneAttack::UBTTask_DroneAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Drone Attack");
}

/**
 * @brief When node is executed it attacks the train
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_DroneAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller & check AI controller is not nullptr
	if (const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get enemy
		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());

		// if enemy is not nullptr
		if (Enemy)
		{
			// if drone can attack
			if (bCanAttack)
			{
				// Get train
				ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));

				// Get player character
				ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
				
				// If it is the train
				if (Player)
				{
					// attack the train, set can attack to false and start timer
					UE_LOG(LogTemp, Warning, TEXT("drone attack"));

					// call attack
					Enemy->GetAbilitySystemComponent()->TryActivateAbilityByClass(Enemy->QRGAAttack, true);

					UGameplayStatics::PlaySoundAtLocation(GetWorld(),Enemy->DronePewPew, Enemy->GetActorLocation(), FRotator(0,0,0), 0.5f);
					
					bCanAttack = false;
					GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &UBTTask_DroneAttack::SetCanAttack, AttackDelay, false);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("notrain"));
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
void UBTTask_DroneAttack::SetCanAttack()
{
	// can attack
	bCanAttack = true;

	// clear timer
	GetWorld()->GetTimerManager().ClearTimer(AttackDelayHandle);
}
