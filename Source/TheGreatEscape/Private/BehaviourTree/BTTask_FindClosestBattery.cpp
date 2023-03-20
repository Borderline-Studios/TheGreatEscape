// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_CloestBattery.cpp
// Description : Checks if a battery exists & sets closest location
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_FindClosestBattery.h"
#include "EnemyRework.h"
#include "EnemyReworkController.h"
#include "NetworkMessage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_FindClosestBattery::UBTTask_FindClosestBattery(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Closest Battery");

	// find and set the class to check
	BatteryRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Interactables/Blueprints/BP_BatteryPickUp.BP_BatteryPickUp_C'")));
	BatteryClass = BatteryRef.LoadSynchronous();
}

/**
 * @brief When node is executed it finds the next patrol point 
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_FindClosestBattery::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller  & if not nullptr
	if (AEnemyReworkController* const AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{

		// Get enemy
		AEnemyRework* const Enemy = Cast<AEnemyRework>(AIController->GetPawn());
		
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), BatteryClass, FoundActors);
		

		if (FoundActors.Num() == 0)
		{
			AIController->GetBlackboard()->SetValueAsBool(BbKeys::bBatteryNearby, false);
		}
		else
		{
			clostestDistance = 99999999.0f;
			
			for (int i = 0; i < FoundActors.Num(); i++)
			{
				if (Enemy->GetDistanceTo(FoundActors[i]) < clostestDistance)
				{
					clostestDistance = Enemy->GetDistanceTo(FoundActors[i]);
					ClosestBattery = FoundActors[i];
				}
			}

			if (clostestDistance < BatteryRangeCheck)
			{
				AIController->GetBlackboard()->SetValueAsVector(BbKeys::batteryLocation, ClosestBattery->GetActorLocation());
				UE_LOG(LogTemp, Warning, TEXT("closest bat is %s"), *ClosestBattery->GetName());
			}
			else
			{
				AIController->GetBlackboard()->SetValueAsBool(BbKeys::bBatteryNearby, false);
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
