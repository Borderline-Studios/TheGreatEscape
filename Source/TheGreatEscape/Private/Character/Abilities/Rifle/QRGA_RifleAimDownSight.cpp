// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Rifle/QRGA_RifleAimDownSight.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_RifleAimDownSight::UQRGA_RifleAimDownSight()
{
	AbilityInputID = EGASAbilityInputID::AimDownSights;
}

void UQRGA_RifleAimDownSight::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if(GetPlayerReference()->bRifleEquipped)
	{
		InputRelaese = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		InputRelaese->OnRelease.AddUniqueDynamic(this, &UQRGA_RifleAimDownSight::ReleasedInput);
		InputRelaese->ReadyForActivation();

		GetPlayerReference()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("AimIn");
		GetPlayerReference()->RifleMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RifleAimDownSight::CallEndAbility);
		 
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UQRGA_RifleAimDownSight::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_RifleAimDownSight::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_RifleAimDownSight::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_RifleAimDownSight::ReleasedInput(float TimePressed)
{
	GetPlayerReference()->bADS = false;
	GetPlayerReference()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("AimOut");
	GetPlayerReference()->RifleMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RifleAimDownSight::CallEndAbility);
}

void UQRGA_RifleAimDownSight::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("AimInFinished"))
	{
		GetPlayerReference()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("HoldADS");
		GetPlayerReference()->bADS = true;
	}
	if (NotifyName == FName("AimOutFinished"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

