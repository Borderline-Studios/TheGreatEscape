// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Rifle/QRGA_RifleShoot.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Abilities/QRGA_Crouch.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_RifleShoot::UQRGA_RifleShoot()
{
	AbilityInputID = EGASAbilityInputID::Shoot;
}

void UQRGA_RifleShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	InputRelaese = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRelaese->OnRelease.AddDynamic(this, &UQRGA_RifleShoot::ReleasedInput);
	InputRelaese->ReadyForActivation();
	//float TimerInRate = 60 / FireRate;
	//GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &UQRGA_RifleShoot::FireLoop, TimerInRate, true, -1);
	FireLoop();
}

void UQRGA_RifleShoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

bool UQRGA_RifleShoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_RifleShoot::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_RifleShoot::ReleasedInput(float TimePressed)
{
	bInputReleased = true;
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_RifleShoot::FireLoop()
{
	if (GetPlayerReference()->CurrentRifleAmmo > 0)
	{
		GetPlayerReference()->CurrentRifleAmmo--;
		GetPlayerReference()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("Fire");
		GetPlayerReference()->RifleMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RifleShoot::CallEndAbility);
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

}

void UQRGA_RifleShoot::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("FireFinished"))
	{
		if (GetPlayerReference()->CurrentRifleAmmo <= 0)
		{
			//Ends ability is the animation is done and ammo is out
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
		else if(GetPlayerReference()->CurrentRifleAmmo > 0)
		{
			FireLoop();
		}
	}
}
