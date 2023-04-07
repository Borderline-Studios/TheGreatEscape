// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Sprint.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Abilities/Data.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GameFramework/CharacterMovementComponent.h"

//Constructor
UQRGA_Sprint::UQRGA_Sprint()
{
	//Sets the Input ID for the ability and links in via unreal back end to the input axis/actions
	AbilityInputID = EGASAbilityInputID::Sprint;
}

//When the Input ID is pressed this function will be called and run, it is the meat of the abiltities
void UQRGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	InputRelaese = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRelaese->OnRelease.AddDynamic(this, &UQRGA_Sprint::ReleasedInput);
	InputRelaese->ReadyForActivation();

	float SprintSpeed = GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed + 150.0f;

	GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Deactivate");
	//Added dynamic notify and triggers function if notify is received
	GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Sprint::CallEndAbility);
}

void UQRGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

/**
 * @brief Gets a reference to the PlayerCharacter class
 * @return Returns a Character Reference which is of Type APlayerCharacter
 */
APlayerCharacter* UQRGA_Sprint::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_Sprint::ReleasedInput(float TimePressed)
{
	float RunSpeed = GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed - 150.0f;

	GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Activate");
	//Added dynamic notify and triggers function if notify is received
	GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Sprint::CallEndAbility);
	

}

void UQRGA_Sprint::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("FinishedGather"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	if (NotifyName == FName("Gather"))
	{
		GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("SprintHold");
	}
}
