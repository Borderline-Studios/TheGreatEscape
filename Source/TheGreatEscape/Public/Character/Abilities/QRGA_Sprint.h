// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_Sprint.h
// Description : The header file for the Sprint ability
// Author      : Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once



//Includes
#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_Sprint.generated.h"

//Forward Declarations
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_Sprint : public UQRGameplayAbility
{
	GENERATED_BODY()


public:
	//Functions and Constructor
	
	UQRGA_Sprint();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//Gets a Reference to the player
	APlayerCharacter* GetPlayerReference();

private:
	int RunFOV = 90;
	int SprintFOV = 100;
};
