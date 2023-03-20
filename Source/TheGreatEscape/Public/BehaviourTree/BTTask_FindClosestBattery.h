// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_CloestBattery.h
// Description : Checks if a battery exists & sets closest location
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindClosestBattery.generated.h"

/**
 * Task to find closest battery
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_FindClosestBattery : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTTask_FindClosestBattery(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // When the task is called this function is called

protected:
	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowProtectedAccess = "true"))
	float BatteryRangeCheck = 1000.0f;

private:
	// *** Variables *** ///
	TSoftClassPtr<AActor> BatteryRef;
	UClass* BatteryClass;
	AActor* ClosestBattery;
	float clostestDistance;
};
