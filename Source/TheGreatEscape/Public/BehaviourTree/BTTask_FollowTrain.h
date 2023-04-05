// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FollowTrain.h
// Description : Follows train
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FollowTrain.generated.h"

/**
 * task to follow train
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_FollowTrain : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTTask_FollowTrain(FObjectInitializer const& ObjectInitializer); // constructor
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // When the task is called this function is called

private:
	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone", meta = (AllowPrivateAccess = "true"))
	float EvelvationHeight = 200.0f; // height above train the enemy will float

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone", meta = (AllowPrivateAccess = "true"))
	float ErrorAmount = 0.1f; // Range of error

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone", meta = (AllowPrivateAccess = "true"))
	float SlowingDist = 1500.0; // Distance to slow down
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone", meta = (AllowPrivateAccess = "true"))
    float StoppingDist = 1.0; // Distance to slow down

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone", meta = (AllowPrivateAccess = "true"))
	float MinTime = 0.05;

	float Speed = 1.0f; // speed of drone

	float DistToTrain = 0.0f;
}; 
