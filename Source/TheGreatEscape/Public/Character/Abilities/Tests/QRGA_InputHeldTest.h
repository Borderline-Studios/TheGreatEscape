// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_InputHeldTest.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_InputHeldTest : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	UQRGA_InputHeldTest();

	UPROPERTY()
	class UAbilityTask_WaitInputRelease* InputRelaese;
	
	//GAS functions for adding logic to abilities
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	UFUNCTION()
	void ReleasedInput(float TimePressed);
	void AbilityLogic();

	UFUNCTION()
	void OtherStuff();

	float TimeDelay = 0.5f;
};
