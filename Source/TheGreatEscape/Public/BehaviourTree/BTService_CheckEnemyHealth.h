// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_CheckEnemyHealth.h
// Description : Checks whether train or player is closest to the target
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckEnemyHealth.generated.h"

/**
 * service to check health of enemy
 */
UCLASS()
class THEGREATESCAPE_API UBTService_CheckEnemyHealth : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckEnemyHealth();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // called when node is active

protected:
	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowProtectedAccess = "true"))
	float HealthToCheck = 60.0f;
};
