// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_TrainInRange.h
// Description : Checks in player is in melee range
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_TrainInRange.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UBTService_TrainInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_TrainInRange();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.0f;
};
