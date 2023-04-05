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

		//UE_LOG(LogTemp, Warning, TEXT("enemy: %s"), *Enemy->GetName());

		// if train not nullptr
		if (Player)
		{
			// locations
			ATrainCarriage* CarriageRef = Train->GetLastCarriage();

			FVector TrainLocation;
			
			if (CarriageRef)
			{
				TrainLocation = CarriageRef->GetActorLocation();
			}
			else
			{
				TrainLocation = Train->GetActorLocation();
			}
			
			FVector EnemyLocation = Enemy->GetActorLocation();

			FVector TrainLocWithOffset = TrainLocation + Enemy->TrainTargetPointOffset;

			//UE_LOG(LogTemp, Warning, TEXT("TrainTargetPointOffset: %s"), *Enemy->TrainTargetPointOffset.ToString());

			FVector direction = TrainLocWithOffset - EnemyLocation;
			//FVector direction = PlayerLocWithOffset - EnemyLocation;
			
//
			// Get distance to train
			DistToTrain = direction.Size();
			
			direction.Normalize();
//
			if (Enemy->Time <= MinTime)
			{
				Enemy->Time = MinTime;
			}
			// Calculate speed based on distance to train
			Speed = DistToTrain / Enemy->Time;

			UE_LOG(LogTemp, Warning, TEXT("timmmmmee %f"), Enemy->Time);
			//UE_LOG(LogTemp, Warning, TEXT("Speed: %d"), Speed);

			// SET VELOCITY
			direction *= Speed;
			// set velocity of enemy
			Enemy->GetMovementComponent()->Velocity = direction;

			//UE_LOG(LogTemp, Warning, TEXT("dist %f"), DistToTrain);
//
			// If within 'stopping' range
			if (DistToTrain <= StoppingDist)
			{
				UE_LOG(LogTemp, Warning, TEXT("STOPP BITCH"));
				Enemy->Time += Enemy->TimeIncrement;
			}
			// If within 'slowing' range, adjust speed based on distance and try to match speed of train
			else if (DistToTrain <= SlowingDist)
			{
				//UE_LOG(LogTemp, Warning, TEXT("SLOWWW"));
				// check if within error range
				if (DistToTrain <= Enemy->LastDist + ErrorAmount && DistToTrain >= Enemy->LastDist - ErrorAmount)
				{
					Enemy->LastDist = DistToTrain;
					UE_LOG(LogTemp, Warning, TEXT("in the error range"));
				}
				else if (DistToTrain < Enemy->LastDist)
				{
					Enemy->Time += Enemy->TimeIncrement;
					UE_LOG(LogTemp, Warning, TEXT("slowing down"));
				}
				else if (DistToTrain > Enemy->LastDist)
				{
					Enemy->Time -= Enemy->TimeIncrement;
					UE_LOG(LogTemp, Warning, TEXT("speeding up"));
				}
			}
			else
			{
				Enemy->Time -= Enemy->TimeIncrement;
				//UE_LOG(LogTemp, Warning, TEXT("speeding up not sd"));
			}
			
			Enemy->LastDist = DistToTrain;

			
			
			 //if direction < slowing dist set vel to slower move dist (train speed)
			 //otherwise have 2 radius 1 to slow 1 to stop
			//if (FVector::Dist(TrainLocWithOffset, EnemyLocation) <= StoppingDist) // FIX
			//{
			//	//direction.Z *= 2.0f;
			//	direction *= 0;
			//	//UE_LOG(LogTemp, Warning, TEXT("im an issue"));
			//}
			//else if (FVector::Dist(TrainLocWithOffset, EnemyLocation) <= SlowingDist)
			//{
			//	direction.Z *= 3.0f;
			//	direction *= Speed / 1.7f;
			//}
			//else
			//{
			//	// multiple direction by speed
			//	direction.Z *= 10.0f;
			//	direction *= Speed;
			//}
			
		

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
