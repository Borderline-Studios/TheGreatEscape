// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_ADSTest.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_ADSTest : public UQRGameplayAbility
{
	GENERATED_BODY()
public:
	UQRGA_ADSTest();

	//GAS back end function for added logic to the fucntions
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//Function to return a reference to the player for access persistence player values.
	APlayerCharacter* GetPlayerReference();

	void CallEndAbility(float TimeHeld);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsHeldDown = false;

	FTimerHandle TimerHandle;
};
