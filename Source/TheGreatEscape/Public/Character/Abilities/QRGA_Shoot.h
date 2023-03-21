// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_Shoot.h
// Description : The header file for the shoot ability
// Author      : Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once
//Includes
#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include <iostream>
#include <random>
#include "QRGA_Shoot.generated.h"

//Forward Declearations
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_Shoot : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	//Constructor	
	UQRGA_Shoot();

	//GAS functions for adding Logic to abilities
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//TODO No Longer needed remember to remove
	void ToggleShooting();

	//Function to return a reference to the player for access persistence player values.
	APlayerCharacter* GetPlayerReference();

	//TODO Remove this handle (Not nedded)
	FTimerHandle ShootHandle;

	FRandomStream Stream;

	//Declearation of effect to apply when enemy is hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	//Animation Montage declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Animation)
	UAnimMontage* AM_Fire;

	//Sound effect declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SFX)
	USoundBase* ShootSFX;

	//Niagara VFX declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	UNiagaraSystem* HitVFX;

	//Function that animation nofity will call 
	UFUNCTION()
	void CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
};

