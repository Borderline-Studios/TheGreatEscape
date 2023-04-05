// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_RevolverEquip.generated.h"

//Forward Decleartions
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_RevolverEquip : public UQRGameplayAbility
{
	GENERATED_BODY()

	UQRGA_RevolverEquip();

	//GAS functions for adding logic to abilities
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	//Function to return a reference to the player for access persistence player values.
	APlayerCharacter* GetPlayerReference();

	//UFunctions
	UFUNCTION()
	void CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
};