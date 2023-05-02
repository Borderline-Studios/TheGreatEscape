// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Rifle/QRGA_RifleReload.h"

#include "Character/Player/PlayerCharacter.h"

UQRGA_RifleReload::UQRGA_RifleReload()
{
	AbilityInputID = EGASAbilityInputID::Reload;
}

void UQRGA_RifleReload::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (GetPlayerReferance()->bRifleEquipped)
	GetPlayerReferance()->CurrentRifleAmmo = 0;
	GetPlayerReferance()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("Reload");
	GetPlayerReferance()->RifleMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RifleReload::CallEndAbility);
}

void UQRGA_RifleReload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_RifleReload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_RifleReload::GetPlayerReferance()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_RifleReload::CallEndAbility(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("ReloadFinished"))
	{
		GetPlayerReferance()->CurrentRifleAmmo = GetPlayerReferance()->RifleAmmo;
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}
