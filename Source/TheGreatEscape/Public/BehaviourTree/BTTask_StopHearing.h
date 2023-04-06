// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_StopHearing.h
// Description : Stops the enemy hearing an old sound
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_StopHearing.generated.h"

/**
 * stops enemy hearing an old sound
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_StopHearing : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTTask_StopHearing(FObjectInitializer const& ObjectInitializer); // constructor
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // When the task is called this function is called 
};
