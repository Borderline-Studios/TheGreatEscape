// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_HybridAttack.h
// Description : Attack functionality for the hybrid
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "HybridEnemyProjectile.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_HybridAttack.generated.h"

/**
 * Task to make hybrid enemy attack
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_HybridAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// *** Functions *** //
	UBTTask_HybridAttack(FObjectInitializer const& ObjectInitializer); // constructor
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // When the task is called this function is called

	private:
	// *** Functions *** ///
	UFUNCTION() 
	void SetCanAttack(); // reset attack bool & timer

	// *** Variables *** ///
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSoftClassPtr<AHybridEnemyProjectile> ProjectileClass; // weak pointer to class type we want

	UClass* LoadedBpProjectile; // Class to spawn
	
	FTimerHandle AttackDelayHandle; // Timer handle to handle the attack delay
	bool bCanAttack = true; // bool to check if drone can attack

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackDelay = 3.0f; // Delay between attacks

};