// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_Interact.h
// Description : The header file for the interact ability
// Author      : Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once
//includes
#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_Interact.generated.h"

//Forward Declearation
class APlayerCharacter;
class UQRGA_RifleEquip; 
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_Interact : public UQRGameplayAbility
{
	GENERATED_BODY()
public:
	//Constructor
	UQRGA_Interact();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> HealingEffectClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundBase*> HealSFX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundBase*> FullHealSFX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* PickUpRifleSFX;
	
	
	
	//GAS back end function for added logic to the fucntions
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//Function to return a reference to the player for access persistence player values.
	APlayerCharacter* GetPlayerReferance();

	UFUNCTION()
	void CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void ForceEndAbility();

	
	void FirstRifleEqiup();

	UFUNCTION(BlueprintCallable)
	void FirstRevovlerEqiup();

	AActor* ActorToDestory;

	FTimerHandle ForceEndHandle;
};
