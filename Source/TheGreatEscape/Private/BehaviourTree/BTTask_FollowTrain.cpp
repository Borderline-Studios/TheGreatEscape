// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FollowTrain.cpp
// Description : Follows the train
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_FollowTrain.h"
#include "EnemyReworkController.h"
#include "EnemyReworkDrone.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TrainEngine.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_FollowTrain::UBTTask_FollowTrain(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Follow Train");
}

/**
 * @brief When node is executed it follows the train
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_FollowTrain::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get controller
	AEnemyReworkController* const  AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner());

	// If ai controller not empty
	if (AIController)
	{
		// get train engine
		ATrainEngine* Train = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
		
		// player
		ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		// get enemy
		AEnemyReworkDrone* const Enemy = Cast<AEnemyReworkDrone>(AIController->GetPawn());

		// if train not nullptr
		if (Player)
		{
			// locations
			FVector TrainLocation = Train->GetActorLocation();
			FVector EnemyLocation = Enemy->GetActorLocation();
			//FVector PlayerLocation = Player->GetActorLocation(); // temp

			FVector TrainLocWithOffset = TrainLocation + Enemy->TrainTargetPointOffset;
			//FVector PlayerLocWithOffset = PlayerLocation + FVector(0.0f, 0.0f, EvelvationHeight);

			FVector direction = TrainLocWithOffset - EnemyLocation;
			//FVector direction = PlayerLocWithOffset - EnemyLocation;
			
			direction.Normalize();
			
			// if direction < slowing dist set vel to slower move dist (train speed)
			// otherwise have 2 radius 1 to slow 1 to stop
			if (FVector::Dist(TrainLocWithOffset, EnemyLocation) <= StoppingDist) // FIX
			{
				//direction.Z *= 2.0f;
				direction *= 0;
				UE_LOG(LogTemp, Warning, TEXT("im an issue"));
			}
			else if (FVector::Dist(TrainLocWithOffset, EnemyLocation) <= SlowingDist)
			{
				direction.Z *= 3.0f;
				direction *= Speed / 1.7f;
			}
			else
			{
				// multiple direction by speed
				direction.Z *= 10.0f;
				direction *= Speed;
			}
			
			// set velocity of enemy
			Enemy->GetMovementComponent()->Velocity = direction;

			// look at train
			FRotator newTurretBaseRot = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), TrainLocation);
			Enemy->TurretBaseRef->SetRelativeRotation(newTurretBaseRot); // rotate the turret base mesh
			

			//FVector Forward = TrainLocation - EnemyLocation;
			FRotator newDroneRot = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), (Enemy->GetActorLocation() + Enemy->GetVelocity()));
			Enemy->SetActorRotation(newDroneRot);
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cast to ATrainEngine failed"));
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
