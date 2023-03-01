// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviourTree/BTTask_DroneAttack.h"

#include "EnemyReworkDrone.h"
#include "EnemyReworkController.h"
#include "TrainEngine.h"
#include "Kismet/GameplayStatics.h"

UBTTask_DroneAttack::UBTTask_DroneAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Drone Attack");
}

EBTNodeResult::Type UBTTask_DroneAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
				ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

				ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
				
				if (Train)
				{
					UE_LOG(LogTemp, Warning, TEXT("drone attack"));
					//Enemy->Attack(player->GetAbilitySystemComponent());
					bCanAttack = false;
					GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &UBTTask_DroneAttack::SetCanAttack, AttackDelay, false);
					//UE_LOG(LogTemp, Warning, TEXT("Cast to Train is: %s"), *Train->GetName())
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("notrain"));
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

void UBTTask_DroneAttack::SetCanAttack()
{
	// can attack
	bCanAttack = true;

	// clear timer
	GetWorld()->GetTimerManager().ClearTimer(AttackDelayHandle);
}
