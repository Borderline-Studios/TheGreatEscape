// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_NPCMoveToNode.h
// Description : Moves the npc to the node
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_NPCMoveToNode.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_NPCMoveToNode : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_NPCMoveToNode(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRadius = 50.0f;
	
};
