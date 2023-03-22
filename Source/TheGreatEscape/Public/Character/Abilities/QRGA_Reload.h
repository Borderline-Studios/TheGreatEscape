// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_Reload.h
// Description : The header file for the reload ability
// Author      : Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once
//includes
#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_Reload.generated.h"

//Forward Decleartions
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_Reload : public UQRGameplayAbility
{
	GENERATED_BODY()
public:
	//Constructor
	UQRGA_Reload();

	//GAS functions for adding logic to abilities
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//UProperties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SFX)
	USoundBase* ReloadSFX;

	//UFunctions
	UFUNCTION()
	void CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void RemoteActivateAbilitiy();

	FGameplayAbilitySpecHandle HHandle;
	FGameplayAbilityActorInfo* HActorInfo;
	FGameplayAbilityActivationInfo HActivationInfo;
	FGameplayEventData* HTriggerEventData;

	//Function to return a reference to the player for access persistence player values.
	APlayerCharacter* GetPlayerReference();
};


