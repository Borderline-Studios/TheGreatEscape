//// Bachelor of Software Engineering
//// Media Design School
//// Auckland
//// New Zealand
//// (c) 2022 Media Design School
//// File Name   :
//// Description :
//// Author      :  Borderline Studios - (person(s) working on file)
//// Mail        : 


#include "Character/Abilities/Revolver/QRGA_RevolverEquip.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_RevolverEquip::UQRGA_RevolverEquip()
{
	AbilityInputID = EGASAbilityInputID::EquipRevolver;
}

void UQRGA_RevolverEquip::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (GetPlayerReference()->bRevolverEquipped)
	{
		//Ends ability when animation is finished
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else
	{
		GetPlayerReference()->bRifleEquipped = false;
		GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("DeActivate");
		GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_RevolverEquip::CallEndAbility);
	}


}

void UQRGA_RevolverEquip::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetPlayerReference()->bRevolverEquipped = true;
}

bool UQRGA_RevolverEquip::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_RevolverEquip::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_RevolverEquip::CallEndAbility(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//Checks the Nofity name
	if (NotifyName == FName("FinishedGather"))
	{
		//Ends ability when animation is finished
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	if (NotifyName == FName("DeactivateFinished"))
	{
		GetPlayerReference()->RifleMesh1P->SetVisibility(false);
		GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("Activate");
		GetPlayerReference()->RevolverMesh1P->SetVisibility(true);
		//Checks for an Animnotify then triggers function
		GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RevolverEquip::CallEndAbility);
	}
}
