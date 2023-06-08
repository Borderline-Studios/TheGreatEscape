// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_DasherTrackPlayer.h
// Description : Task for dasher to track player and start attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DasherTrackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_DasherTrackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTTask_DasherTrackPlayer(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UFUNCTION()
	void SetCanDash(); // reset attack bool & timer

	FTimerHandle DashDelayHandle; // Timer handle to handle the attack delay
	bool bCanDash = true; // bool to check if can dash
	bool bDashing = false; // bool to check if dashing

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float TrackingTime = 1.0f; // Delay between attacks
	
};
