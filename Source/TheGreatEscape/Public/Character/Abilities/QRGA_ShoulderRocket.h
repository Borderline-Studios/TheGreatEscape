// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "QRGA_ShoulderRocket.generated.h"
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_ShoulderRocket : public UQRGameplayAbility
{
	GENERATED_BODY()

public:
	UQRGA_ShoulderRocket();

	FTimerHandle CooldownTimerHandle;
	
	UPROPERTY()
	class UAbilityTask_WaitInputRelease* InputRelaese;
	
	//GAS functions for adding Logic to abilities
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateCheckProjectile(AActor* InputActor);
	
	//Function to return a reference to the player for access persistence player values.
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerReference();

	UFUNCTION()
	void ReleasedInput(float TimePressed);

	UFUNCTION()
	void CallEndAbility();
	

	UPROPERTY()
	float RocketDistance = 2000.0f;

	UPROPERTY()
	AActor* ClosestActor;

	UPROPERTY()
	bool bActorFound = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownTime = 15.0f;
	
	
};
