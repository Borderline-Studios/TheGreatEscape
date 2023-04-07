// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/QRGA_Crouch.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"

UQRGA_Crouch::UQRGA_Crouch()
{
	AbilityInputID = EGASAbilityInputID::Crouch;
}

void UQRGA_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	InputRelaese = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRelaese->OnRelease.AddDynamic(this, &UQRGA_Crouch::ReleasedInput);
	InputRelaese->ReadyForActivation();
	float CrouchRadSize = GetPlayerReferance()->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float CrouchHalfHeight = GetPlayerReferance()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/2;
	
	GetPlayerReferance()->GetCapsuleComponent()->SetCapsuleSize(CrouchRadSize, CrouchHalfHeight); 
	
}

void UQRGA_Crouch::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_Crouch::GetPlayerReferance()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_Crouch::ReleasedInput(float TimePressed)
{
	float CrouchRadSize = GetPlayerReferance()->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float CrouchHalfHeight = GetPlayerReferance()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2;
	
	GetPlayerReferance()->GetCapsuleComponent()->SetCapsuleSize(CrouchRadSize, CrouchHalfHeight);
	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
