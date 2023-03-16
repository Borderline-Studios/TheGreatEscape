// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_ResourceGather.h
// Description : The header file for the Gather ability
// Author      : Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once

//includes
#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_ResourceGather.generated.h"

//forward declearation
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_ResourceGather : public UQRGameplayAbility
{
	GENERATED_BODY()
	

public:
	//constructor
    UQRGA_ResourceGather();

	//GAS functions for creating logic for abilities
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//Nofity UFunctions
	UFUNCTION()
	void ResourceLogic(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION()
	void CallEndAbilitity(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	//Player reference getter
	APlayerCharacter* GetPlayerReference();
	
};
