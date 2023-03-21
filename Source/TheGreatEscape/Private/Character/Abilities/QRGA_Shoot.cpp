// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Shoot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintNodeHelpers.h"
#include "ShaderCompiler.h"
//#include "../../../../../../../../../../../../Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/INCLUDE/string"
#include "EnemyRework.h"
#include "Camera/CameraComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Kismet/GameplayStatics.h"

#include "Character/QRCharacter.h"
#include "Character/BASE/GASBASECharacter.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interactables/WorldInteractTrigger.h"

UQRGA_Shoot::UQRGA_Shoot()
{
	//sets the input key via the Enum
	AbilityInputID = EGASAbilityInputID::Shoot;
}

void UQRGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	int MaxShotRange = GetPlayerReference()->MaxShotRange;

	
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
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX, CamCompLocation,FRotator(0,0,0), 0.3, 1);

		//Varaibles for Hitscan check
		FHitResult HitResult;
		FCollisionQueryParams Params;
		//Ignore self
		Params.AddIgnoredActor(GetPlayerReference());



		//TODO Simplfy the Compent location and no magic numbers
		//Line trace from crosshair/middle of player screen straight line infront of player
		if (GetWorld()->LineTraceSingleByChannel(HitResult,CamCompLocation,CamCompLocation + CamCompForwardVector * MaxShotRange,ECC_Visibility, Params))
		{
			float CamControlPitch = GetPlayerReference()->GetController()->GetControlRotation().Pitch;
			float CamControlYaw = GetPlayerReference()->GetController()->GetControlRotation().Yaw;
			float CamControlRoll = GetPlayerReference()->GetController()->GetControlRotation().Roll;
			float AimPunchAmount = 2.1;
			
			//Add crosshair recoil (aim punch)
			GetPlayerReference()->GetController()->SetControlRotation(FRotator(CamControlPitch + AimPunchAmount, CamControlYaw, CamControlRoll));
			//Cam Shake
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(GetPlayerReference()->CamShake, 1.0f);
			if (HitResult.GetActor())
			{
				//Getting the ability system component from the hit actor
				UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
				//Check if ASC is vaild
				if(ASC)
				{
					//Creates damage effect outgoing handle
					FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
					//Actiavte hit VFX on hit object/actor
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, HitResult.Location, HitResult.GetActor()->GetActorRotation());
					//play animation
					if (AEnemyRework* Enemy = Cast<AEnemyRework>(HitResult.GetActor()))
					{
						Enemy->GetMesh()->GetAnimInstance()->Montage_JumpToSection("Hit");
					}
					
					//Uses the out going handle to deal damage
					ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
				}
				else if (HitResult.GetActor()->ActorHasTag("Trigger"))
				{
					if (HitResult.GetActor()->ActorHasTag("Bridge"))
					{
						if (AWorldInteractTrigger* WorldInteractTrigger = Cast<AWorldInteractTrigger>(HitResult.GetActor()))
						{
							if (HitResult.GetActor() == WorldInteractTrigger)
							{
								WorldInteractTrigger->MoveActorOnTrigger(); 
							}
						}
					}
				}
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
