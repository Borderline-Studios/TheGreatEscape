// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_MeleeEnemyAttack.h
// Description : Gameplay ability to have melee enemy attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#pragma once

#include "CoreMinimal.h"
#include "EnemyRework.h"
#include "QRGameplayAbility.h"
#include "Character/Player/PlayerCharacter.h"
#include "QRGA_MeleeEnemyAttack.generated.h"

/**
 *  gameplay ability to get the melee enemy to attack
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_MeleeEnemyAttack : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	UQRGA_MeleeEnemyAttack(); // constructor
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	AEnemyRework* GetEnemyRef(); // Gets a reference to the enemy drone

	// *** Variables *** ///
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass; // What effect it is
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	float LineTraceMultiplier = 1000.0f; // How far the line trace will go

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<USoundBase>> PlayerHitEffects;
};
