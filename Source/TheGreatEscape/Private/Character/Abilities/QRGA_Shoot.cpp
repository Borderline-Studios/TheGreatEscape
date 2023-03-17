// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Shoot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintNodeHelpers.h"
#include "ShaderCompiler.h"
//#include "../../../../../../../../../../../../Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/INCLUDE/string"
#include "Camera/CameraComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Kismet/GameplayStatics.h"

#include "Character/QRCharacter.h"
#include "Character/BASE/GASBASECharacter.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_Shoot::UQRGA_Shoot()
{
	//sets the input key via the Enum
	AbilityInputID = EGASAbilityInputID::Shoot;
}

void UQRGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//Checks is ammo is depleated
	if(GetPlayerReference()->PlayerAmmo <= 0)
	{
		//If ammo is 0 end ability
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else
	{
		if (GetPlayerReference()->bIsADS)
		{
			//Jumps the animontage to the fire section
			GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("FireADS");
			//Added dynamic notify and triggers function if notify is received
			GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Shoot::CallEndAbility);
		}
		else
		{
			
			//Jumps the animontage to the fire section
			GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Fire");
			//Added dynamic notify and triggers function if notify is received
			GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Shoot::CallEndAbility);
		}
		
		//Decrements the Ammo
		GetPlayerReference()->PlayerAmmo--;

		//Plays the sound at the player
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX,
											  GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
											  FRotator(0,0,0), 0.3, 1);

		//Varaibles for Hitscan check
		FHitResult HitResult;
		FCollisionQueryParams Params;
		//Ignore self
		Params.AddIgnoredActor(GetPlayerReference());

		//TODO Simplfy the Compent location and no magic numbers
		//Line trace from crosshair/middle of player screen straight line infront of player
		if (GetWorld()->LineTraceSingleByChannel(HitResult,GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
													   GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation() +
														   GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector() * 20000,
														   ECC_Visibility, Params))
		{
			//Add crosshair recoil (aim punch)
			GetPlayerReference()->GetController()->SetControlRotation(FRotator(GetPlayerReference()->GetController()->GetControlRotation().Pitch + 2.1,
																					 GetPlayerReference()->GetController()->GetControlRotation().Yaw,
																					 GetPlayerReference()->GetController()->GetControlRotation().Roll));
			//Cam Shake
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(GetPlayerReference()->CamShake, 1.0f);
			
			//Getting the ability system component from the hit actor
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
			//Check if ASC is vaild
			if(ASC)
			{
				//Creates damage effect outgoing handle
				FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
				//Actiavte hit VFX on hit object/actor
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, HitResult.Location, HitResult.GetActor()->GetActorRotation());
				//Uses the out going handle to deal damage
				ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
			}
		}
	}
}

void UQRGA_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//GetWorld()->GetTimerManager().ClearTimer(ShootHandle);
}

void UQRGA_Shoot::ToggleShooting()
{
	//TODO Remove this function
}


APlayerCharacter* UQRGA_Shoot::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_Shoot::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//Checks if the Nofity name
	if (NotifyName == FName("FinishedFire"))
	{
		//Ends ability is the animation is done
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	if (NotifyName == FName("ADSFireFinish"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

bool UQRGA_Shoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
