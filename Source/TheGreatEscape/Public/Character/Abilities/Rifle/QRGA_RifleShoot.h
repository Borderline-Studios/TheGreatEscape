// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "NiagaraFunctionLibrary.h"
#include "QRGA_RifleShoot.generated.h"
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_RifleShoot : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	UQRGA_RifleShoot();

	UPROPERTY()
	class UAbilityTask_WaitInputRelease* InputRelaese;

	UPROPERTY()
	FTimerHandle ShootTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FireRate = 500;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bInputReleased = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundBase*> ShootSFX;


	//GAS functions for adding Logic to abilities
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
	//Function to return a reference to the player for access persistence player values.
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerReference();
	
	UFUNCTION()
	void ReleasedInput(float TimePressed);

	UFUNCTION()
	void FireLoop();

	//Function that animation nofity will call 
	UFUNCTION()
	void CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	FHitResult HitScan(float MaxDistance);

	UFUNCTION()
	void ActivateEffects(FHitResult HitInput);

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateTraceParticle(FHitResult HitInput);

	UFUNCTION()
	void HitEnemyCheck(FHitResult HitInput);

	//Declearation of effect to apply when enemy is hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	//Niagara VFX declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	UNiagaraSystem* MuzzleVFX;

	//Niagara VFX declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	UNiagaraSystem* TraceVFX;

	//Niagara VFX declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	UNiagaraSystem* HitVFX;

	
};
