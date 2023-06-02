// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_HybridTeleport.cpp
// Description : Teleports to a point in a range
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_HybridTeleport.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyReworkController.h"
#include "EnemyReworkHybrid.h"
#include "NiagaraFunctionLibrary.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviourTree/Utils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_HybridTeleport::UBTTask_HybridTeleport(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Teleport to Random Location");
}

/**
 * @brief When node is executed it finds a random location near the player
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_HybridTeleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get Ai controller and enemy
	const AEnemyReworkController*  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());
	
	if (AIController)
	{
		AEnemyReworkHybrid* const Enemy = Cast<AEnemyReworkHybrid>(AIController->GetPawn());

		// FLocation for enemy to go
		FNavLocation Location;

		// Get nav system and generate random location near the player
		UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		
		if (NavSystem->GetRandomReachablePointInRadius(Enemy->GetActorLocation(), TeleportRadius, Location, nullptr))
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(TeleportVfx, Enemy->GetMesh(), FName(Enemy->GetName()), Enemy->GetActorLocation(), Enemy->GetActorRotation(), EAttachLocation::KeepWorldPosition, true, true);
			Enemy->SetActorLocation(Location.Location);
			AIController->GetBlackboard()->SetValueAsBool(BbKeys::hybirdHit, false);
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
