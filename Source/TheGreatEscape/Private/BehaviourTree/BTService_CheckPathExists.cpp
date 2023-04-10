// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_CheckPathExists.cpp
// Description : Checks whether player is reachable
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTService_CheckPathExists.h"

#include "EnemyReworkController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckPathExists::UBTService_CheckPathExists()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Player Reachable");
}

void UBTService_CheckPathExists::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	// get Ai controller & if AIController is not a nullptr
	if (AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
	{
		// Get Enemy
		APawn* const Enemy = AIController->GetPawn();

		// Get player character
		ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		// If can find nav base
		if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			FVector StartLoc = Enemy->GetActorLocation();
			FVector EndLoc= Player->GetActorLocation();

			FPathFindingQuery QueryParams;
			QueryParams.StartLocation = StartLoc;
			QueryParams.EndLocation = EndLoc;
			QueryParams.NavData =  FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld())->GetDefaultNavDataInstance();
			QueryParams.SetAllowPartialPaths(false);

			if (NavSystem->TestPathSync(QueryParams, EPathFindingMode::Hierarchical))
			{
				AIController->GetBlackboard()->SetValueAsBool(BbKeys::playerPathExists, true);
				//UE_LOG(LogTemp, Warning, TEXT("i can see u"));
			}
			else
			{
				AIController->GetBlackboard()->SetValueAsBool(BbKeys::playerPathExists, false);
				//UE_LOG(LogTemp, Warning, TEXT("no path 2 be found"));
			}
		}
		
	}
}
