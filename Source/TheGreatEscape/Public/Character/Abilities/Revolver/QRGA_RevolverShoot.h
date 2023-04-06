// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "NiagaraFunctionLibrary.h"
#include "QRGA_RevolverShoot.generated.h"

//Forward Declearations
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_RevolverShoot : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	UQRGA_RevolverShoot();

	//GAS functions for adding Logic to abilities
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	//Function to return a reference to the player for access persistence player values.
	APlayerCharacter* GetPlayerReference();
	

	FRandomStream Stream;

	//Declearation of effect to apply when enemy is hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	//Sound effect declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SFX)
	TArray<USoundBase*> ShootSFX;

	//Sound effect declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SFX)
	TArray<USoundBase*> EmptySFX;

	//Niagara VFX declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	UNiagaraSystem* HitVFX;
	
	//Niagara VFX declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	UNiagaraSystem* MuzzleVFX;
	
	FTimerHandle ShootForceEndTimer;
	

	//Function that animation nofity will call 
	UFUNCTION()
	void CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void ForceEndAbility();

	//Activates the SFX and VFX
	UFUNCTION()
	void ActivateEffects();

	UFUNCTION()
	FHitResult HitScan(float MaxDistance);

	UFUNCTION()
	void HitEnemyCheck(FHitResult HitInput);

	UFUNCTION()
	void HitTagCheck(FHitResult HitInput);
	
};
