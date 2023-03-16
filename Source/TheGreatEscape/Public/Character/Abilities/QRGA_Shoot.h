// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "QRGA_Shoot.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_Shoot : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	UQRGA_Shoot();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;


	void ToggleShooting();
	APlayerCharacter* GetPlayerReference();
	
	FTimerHandle ShootHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	UAnimMontage* AM_Fire;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SFX)
	USoundBase* ShootSFX;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	UNiagaraSystem* HitVFX;

	UFUNCTION()
	void CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
};

