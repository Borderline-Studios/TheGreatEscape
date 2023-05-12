// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Rifle/QRGA_RifleEquip.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_RifleEquip::UQRGA_RifleEquip()
{
	AbilityInputID = EGASAbilityInputID::EquipRifle;
}

void UQRGA_RifleEquip::CallForceEndTimer(FTimerHandle InputHandle)
{
	GetWorld()->GetTimerManager().SetTimer(InputHandle, this, &UQRGA_RifleEquip::CallForceEndAbility, 10.0f);
}

void UQRGA_RifleEquip::CallForceEndAbility()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_RifleEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (GetPlayerReferance()->bRiflePickedUp && !GetPlayerReferance()->bRifleEquipped)
	{
		GetPlayerReferance()->bRevolverEquipped = false;
		GetPlayerReferance()->bRifleEquipped = true;
		GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("DeActivate");
		GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RifleEquip::CallEndAbility);
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

}

void UQRGA_RifleEquip::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_RifleEquip::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_RifleEquip::GetPlayerReferance()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_RifleEquip::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("Gather"))
	{
		GetPlayerReferance()->RevolverMesh1P->SetVisibility(false);
		GetPlayerReferance()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("Activate");
		GetPlayerReferance()->RifleMesh1P->SetVisibility(true);
		GetPlayerReferance()->RifleMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RifleEquip::CallEndAbility);
	}
	if (NotifyName == FName("Activated"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}
