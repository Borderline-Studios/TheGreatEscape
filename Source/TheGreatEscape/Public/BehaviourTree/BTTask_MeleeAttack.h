// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_MeleeAttack.h
// Description : Melee attack the player
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnemyRework.h"
#include "BTTask_MeleeAttack.generated.h"


/**
 * Behaviour Tree task for the drone attack
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_MeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UBTTask_MeleeAttack(FObjectInitializer const& ObjectInitializer); // constructor
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // When the task is called this function is called 
	
private:
	// *** Functions *** ///
	UFUNCTION()
	void SetCanAttack(); // reset attack bool & timer

	UFUNCTION()
	void WreckerAttack(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	// *** Variables *** ///
	FTimerHandle AttackDelayHandle; // Timer handle to handle the attack delay
	bool bCanAttack = true; // bool to check if drone can attack
	bool bCanHit = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackDelay = 3.0f; // Delay between attacks

	// Rand num
	
	int RandNum = 0;
	
};
