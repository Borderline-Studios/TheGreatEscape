// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_LookAtPlayer.h
// Description : Task to look at the player
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_LookAtPlayer.generated.h"

/**
 * Task to look at the player
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_LookAtPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTTask_LookAtPlayer(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// *** Variables *** ///
	FRotator OriginalRotation; // original rotation of npc
	bool bOriginalRotSet = false; // Original rotation has been set
	float RotationSpeed = 5.0f; // speed of rotation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone", meta = (AllowPrivateAccess = "true"))
	bool bDrone = false;
};
