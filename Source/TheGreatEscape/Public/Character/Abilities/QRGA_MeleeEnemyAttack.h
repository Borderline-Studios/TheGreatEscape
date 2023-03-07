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
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_MeleeEnemyAttack : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	UQRGA_MeleeEnemyAttack(); // constructor
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	AEnemyRework* GetEnemyRef();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LineTrace")
	float LineTraceMultiplier = 1000.0f;
};
