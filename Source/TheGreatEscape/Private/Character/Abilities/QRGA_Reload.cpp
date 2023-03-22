// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Reload.h"

#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_Reload::UQRGA_Reload()
{
	//Setting the input key via the enum
	AbilityInputID = EGASAbilityInputID::Reload;
}

void UQRGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!GetPlayerReference()->bIsADS)
	{
		//Checks if ammo is full
		if (GetPlayerReference()->PlayerAmmo == 6)
		{
			//End ability if ammo is full (Reload no needed)
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
		else
		{
			//Sets ammo to zero to avoid any issues (Probably not nessessary)
			GetPlayerReference()->PlayerAmmo = 0;
			//Sets ammo to full
			GetPlayerReference()->PlayerAmmo = 6;

			//Jumps animontage ot the reload section to player reload animation
			GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Reload");
			//Checks for an Animnotify then triggers function
			GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Reload::CallEndAbility);

			//Plays reload sound at location
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSFX ,
												  GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
												  FRotator(0,0,0), 0.3, 1);
		}
	}

	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	//EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_Reload::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_Reload::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_Reload::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//Checks the Nofity name
	if (NotifyName == FName("FinishedReload"))
	{
		//Ends ability when animation is finished
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	
}

void UQRGA_Reload::RemoteActivateAbilitiy()
{
	
}
