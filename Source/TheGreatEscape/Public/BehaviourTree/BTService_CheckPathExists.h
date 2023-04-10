// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_CheckPathExists.h
// Description : Checks whether player is reachable
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckPathExists.generated.h"

/**
 * checks if the path to player is reachable
 */
UCLASS()
class THEGREATESCAPE_API UBTService_CheckPathExists : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	// *** Functions *** ///
	UBTService_CheckPathExists(); // constructor
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // called when node is active
	
};
