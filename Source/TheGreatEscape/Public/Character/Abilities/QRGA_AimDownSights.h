// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_AimDownSights.h
// Description : Header file for the Aim down sight ability (Child class of QRGameplayAbility)
// Author      : Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once

//Includes
#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_AimDownSights.generated.h"

//Forward Declaration player for getting a reference
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_AimDownSights : public UQRGameplayAbility
{
	GENERATED_BODY()
public:
	//Constructor
	UQRGA_AimDownSights();

	//GAS back end abilities where logic is added to make abilities
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//Function to return a reference to the player for access persistence player values.
	APlayerCharacter* GetPlayerReferance();
};

