// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_FindTrain.h
// Description : Finds train location
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindTrain.generated.h"

/**
 * Task to find train
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_FindTrain : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	// *** Functions *** ///
	UBTTask_FindTrain(FObjectInitializer const& ObjectInitializer); // constructor
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // When the task is called this function is called
	
	
};
