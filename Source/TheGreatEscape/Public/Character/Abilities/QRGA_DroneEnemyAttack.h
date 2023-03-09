// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_DroneEnemyAttack.h
// Description : Gameplay ability to have drone enemy attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "EnemyReworkDrone.h"
#include "QRGA_DroneEnemyAttack.generated.h"

/**
 * Gameplay ability to get the drone to do damage
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_DroneEnemyAttack : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UQRGA_DroneEnemyAttack(); // constructor
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	AEnemyReworkDrone* GetEnemyRef(); // Gets a reference to the enemy drone

	// *** Variables *** ///
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass; // What effect it is
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	float LineTraceMultiplier = 5000.0f; // How far the line trace will go
	
};
