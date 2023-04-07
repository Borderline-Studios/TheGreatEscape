// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "QRGameplayAbility.h"
#include "QRGA_Fanning.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGA_Fanning : public UQRGameplayAbility
{
	GENERATED_BODY()
public:
	UQRGA_Fanning();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	//Gets a Reference to the player
	APlayerCharacter* GetPlayerReference();

	
	FTimerHandle FanHandle;

	UFUNCTION()
	void FanLoop();

	//Sound effect declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = SFX)
	TArray<USoundBase*> ShootSFX;

	//Function that animation nofity will call 
	UFUNCTION()
	void CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	int ShotsRemaining = 0;
	
};
