// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_PlayerInRange.h
// Description : Checks in player is in melee range
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerInRange.generated.h"

/**
 * Service to check a bool
 */
UCLASS()
class THEGREATESCAPE_API UBTService_PlayerInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTService_PlayerInRange(); // constructor
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // called when node is active

private:
	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float MeleeRange = 300.0f; // Range the enemy can attack in
};
