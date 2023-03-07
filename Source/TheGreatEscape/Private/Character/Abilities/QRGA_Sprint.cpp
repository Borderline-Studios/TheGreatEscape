// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Sprint.h"

#include "Character/Player/PlayerCharacter.h"
#include "Character/Abilities/Data.h"
#include "GameFramework/CharacterMovementComponent.h"

UQRGA_Sprint::UQRGA_Sprint()
{
	AbilityInputID = EGASAbilityInputID::Sprint;
}

void UQRGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		
	if (ToggleSprint)
	{
		GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed = GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed * 2;
		ToggleSprint = false;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s = ToogleSprint"), ToggleSprint));
	}
	else if (!ToggleSprint)
	{
		GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed = GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed / 2;
		ToggleSprint = true;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s = ToogleSprint"), ToggleSprint));
		
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f = FloatVariable"), GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed ));

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
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

APlayerCharacter* UQRGA_Sprint::GetPlayerReference()
{
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	return CharacterRef;
}
