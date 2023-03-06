// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTService_NPCInRange.h
// Description : Checks if npc in player range
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_NPCInRange.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UBTService_NPCInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_NPCInRange();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float PlayerRange = 250.0f;
};
