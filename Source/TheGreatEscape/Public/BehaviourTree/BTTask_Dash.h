// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_Dash.h
// Description : The dash attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Dash.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_Dash : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	// *** Functions *** ///
	UBTTask_Dash(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void Dash(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	bool bDashStart = false; // bool to check if dashing

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float DashSpeed = 700.0f; // Dash speed

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float StoppingRange = 50.0f; // Dash speed
	
};
