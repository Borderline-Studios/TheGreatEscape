// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_IncrementPathIndex.h
// Description : increment the path index
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_IncrementPathIndex.generated.h"

/**
 *  task to increment the patrol path index
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_IncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer); // constructor 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // When the task is called this function is called

private:
	// *** Variables *** ///
	enum class EDirectionType // enum for direction of NPC
	{
		Forward,
		Backwards
	};

	EDirectionType Direction = EDirectionType::Forward; // set default as forwards
};
