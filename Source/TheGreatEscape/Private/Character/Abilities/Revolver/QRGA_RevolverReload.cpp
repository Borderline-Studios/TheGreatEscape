// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Revolver/QRGA_RevolverReload.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_RevolverReload::UQRGA_RevolverReload()
{
	//Setting the input key via the enum
	AbilityInputID = EGASAbilityInputID::Reload;
}

void UQRGA_RevolverReload::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetWorld()->GetTimerManager().SetTimer(ReloadForceEndTimer,this,&UQRGA_RevolverReload::ForceEndAbility, 2.0f, true);
	
	if (!GetPlayerReference()->bADS)
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
			GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_RevolverReload::CallEndAbility);

			//Plays reload sound at location
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSFX ,
												  GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
												  FRotator(0,0,0), 0.3, 1);
		}
	}

	else
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
			GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("ADSReload");
			//Checks for an Animnotify then triggers function
			GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_RevolverReload::CallEndAbility);

		
			//Plays reload sound at location
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSFX ,
												  GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
												  FRotator(0,0,0), 0.3, 1);
		}
	
	}
}

void UQRGA_RevolverReload::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetWorld()->GetTimerManager().ClearTimer(ReloadForceEndTimer);
}

bool UQRGA_RevolverReload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UQRGA_RevolverReload::CallEndAbility(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//Checks the Nofity name
	if (NotifyName == FName("FinishedReload"))
	{
		//Ends ability when animation is finished
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

	if (NotifyName == FName("ADSReloadFinished"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UQRGA_RevolverReload::ForceEndAbility()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

APlayerCharacter* UQRGA_RevolverReload::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}
