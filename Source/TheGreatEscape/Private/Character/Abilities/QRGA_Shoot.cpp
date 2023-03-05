// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Shoot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintNodeHelpers.h"
#include "ShaderCompiler.h"
#include "Camera/CameraComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

#include "Character/QRCharacter.h"
#include "Character/BASE/GASBASECharacter.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_Shoot::UQRGA_Shoot()
{
	AbilityInputID = EGASAbilityInputID::Shoot;
}

void UQRGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if(GetPlayerReferance()->PlayerAmmo <= 0)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else
	{
		GetPlayerReferance()->bIsShooting = true;
		GetPlayerReferance()->PlayerAmmo--;
		if(GEngine)
		{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shooting"));
		}
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult,GetPlayerReferance()->GetFirstPersonCameraComponent()->GetComponentLocation(),
													   GetPlayerReferance()->GetFirstPersonCameraComponent()->GetComponentLocation() +
														   GetPlayerReferance()->GetFirstPersonCameraComponent()->GetForwardVector() * 20000,
														   ECC_Visibility))
		{
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
	
			if(ASC)
			{
				FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
				ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
			}
		}
	}
	
	if (!GetPlayerReferance()->bIsShooting)
	{
		GetPlayerReferance()->bIsShooting = false;
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);

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
	GetPlayerReferance()->bIsShooting = false;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shit Poo Ass"));
	}
}


APlayerCharacter* UQRGA_Shoot::GetPlayerReferance()
{
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	return CharacterRef;
}

bool UQRGA_Shoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
