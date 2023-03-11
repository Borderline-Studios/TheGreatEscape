// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_CloestTarget.h
// Description : Checks whether train or player is closest to the target
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ClosestTarget.generated.h"

/**
 * service to check whether train or player is cloest
 */
UCLASS()
class THEGREATESCAPE_API UBTService_ClosestTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTService_ClosestTarget(); // constructor
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // called when node is active

protected:
	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowProtectedAccess = "true"))
	float TrainCheckRange = 100.0f;
};
