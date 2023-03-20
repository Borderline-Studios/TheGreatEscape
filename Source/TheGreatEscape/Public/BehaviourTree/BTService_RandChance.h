// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_RandChance.h
// Description : Has a random chance to return true
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_RandChance.generated.h"

/**
 * Random chance to return true
 */
UCLASS()
class THEGREATESCAPE_API UBTService_RandChance : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_RandChance();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // called when node is active

	protected:
	// *** Variables *** ///
	// Chance of something happening
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowProtectedAccess = "true"))
	float Chance = 3.0f;
	
};
