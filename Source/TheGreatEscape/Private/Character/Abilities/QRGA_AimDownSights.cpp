// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_AimDownSights.h"

#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Evaluation/IMovieSceneEvaluationHook.h"

UQRGA_AimDownSights::UQRGA_AimDownSights()
{
	//Settings the input via the enum
	AbilityInputID = EGASAbilityInputID::AimDownSights;
}

//TODO Make this better (New animations)
void UQRGA_AimDownSights::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!GetPlayerReferance()->bADS)
	{

		GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("ActiADS");
		GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_AimDownSights::NotifyFunction);
	}
	else if (GetPlayerReferance()->bADS)
	{
		GetPlayerReferance()->bADS = false;
		GetPlayerReferance()->GetFirstPersonCameraComponent()->SetFieldOfView(90.0f);
		GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("DeActiADS");
		GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_AimDownSights::NotifyFunction);
	}
}




void UQRGA_AimDownSights::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}





bool UQRGA_AimDownSights::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

/**
 * @brief Returns a reference variable of the player for access persistant variables
 * @return APlayerCharacter reference
 */
APlayerCharacter* UQRGA_AimDownSights::GetPlayerReferance()
{
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	return CharacterRef;
}

void UQRGA_AimDownSights::NotifyFunction(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("ADSFinish"))
	{
	    GetPlayerReferance()->bADS = true;
		GetPlayerReferance()->GetFirstPersonCameraComponent()->SetFieldOfView(50.0f);
		GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("HoldADS");
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	if (NotifyName == FName("DeAdsFinish"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

	
}
