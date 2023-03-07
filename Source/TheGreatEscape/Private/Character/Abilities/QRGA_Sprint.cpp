// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Sprint.h"

#include "Character/Player/PlayerCharacter.h"
#include "Character/Abilities/Data.h"
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

	if (GetPlayerReference()->bIsSprinting)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ToggleSprint = %hs"), GetPlayerReference()->bIsSprinting ?  "True" : "False"));
		GetPlayerReference()->bIsSprinting = false;
		GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed = GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed - 150.0f;
	}
	else if(!GetPlayerReference()->bIsSprinting)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ToggleSprint = %hs"),  GetPlayerReference()->bIsSprinting ?  "TrueF" : "FalseF"));
		GetPlayerReference()->bIsSprinting = true;
		GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed = GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed +  150.0f;
	}
	
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

/**
 * @brief Gets a reference to the PlayerCharacter class
 * @return Returns a Character Reference which is of Type APlayerCharacter
 */
APlayerCharacter* UQRGA_Sprint::GetPlayerReference()
{
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	return CharacterRef;
}
