// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_ResetDash.h
// Description : The dash attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ResetDash.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_ResetDash : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTTask_ResetDash(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float NormalMoveSpeed = 450.0f; // Dash speed
	
};
