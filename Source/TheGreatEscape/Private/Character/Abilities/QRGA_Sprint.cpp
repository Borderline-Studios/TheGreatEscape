// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Sprint.h"

#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Character/Abilities/Data.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

//TODO Clean Up print screens
//TODO Try and make the sprint hold instead of toggle

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

	//TODO Add FOV Lerp to show sprint
	//Checks if player is wanting to sprint
	if (GetPlayerReference()->bSprinting)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ToggleSprint = %hs"), GetPlayerReference()->bIsSprinting ?  "True" : "False"));
		//Setting the Sprint to false
		GetPlayerReference()->bSprinting = false;
		//Decreasing max movement speed
		GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed = GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed - GetPlayerReference()->SprintMod;
		int FOV = GetPlayerReference()->GetFirstPersonCameraComponent()->FieldOfView;
		if(GetPlayerReference()->GetFirstPersonCameraComponent()->FieldOfView != RunFOV)
		{
			while(FOV != RunFOV)
			{
				GetPlayerReference()->GetFirstPersonCameraComponent()->SetFieldOfView(FOV - 1);
				FOV--;
			}
		}
	}
	else if(!GetPlayerReference()->bSprinting)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ToggleSprint = %hs"),  GetPlayerReference()->bIsSprinting ?  "TrueF" : "FalseF"));
		//Setting the Sprint to true
		GetPlayerReference()->bSprinting = true;
		//Increasing max movement speed
		GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed = GetPlayerReference()->GetCharacterMovement()->MaxWalkSpeed +  GetPlayerReference()->SprintMod;
		int FOV = GetPlayerReference()->GetFirstPersonCameraComponent()->FieldOfView;
		if(GetPlayerReference()->GetFirstPersonCameraComponent()->FieldOfView != SprintFOV)
		{
			while(FOV != SprintFOV)
			{
				GetPlayerReference()->GetFirstPersonCameraComponent()->SetFieldOfView(FOV + 1);
				FOV++;
			}
		}

	}

	//Ends ability
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
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}
