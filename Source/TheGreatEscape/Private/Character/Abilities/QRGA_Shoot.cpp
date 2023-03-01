// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Shoot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Camera/CameraComponent.h"

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

	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Ability Acti"));
	}
	

	FHitResult HitResult;
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());

	if (GetWorld()->LineTraceSingleByChannel(HitResult,CharacterRef->GetFirstPersonCameraComponent()->GetComponentLocation(),
	                                               CharacterRef->GetFirstPersonCameraComponent()->GetComponentLocation() + CharacterRef->GetFirstPersonCameraComponent()->GetForwardVector() * 20000,
	                                                   ECC_Visibility))
	{
		DrawDebugLine(GetWorld(),CharacterRef->GetFirstPersonCameraComponent()->GetComponentLocation(),
			CharacterRef->GetFirstPersonCameraComponent()->GetComponentLocation() + CharacterRef->GetFirstPersonCameraComponent()->GetForwardVector() * 20000,
			FColor::Red, false, 5);


		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *HitResult.GetActor()->GetName()));
		}
		
		if(ASC)
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("ASC VALID"));
			}
			FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
			ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
		}
	
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_Shoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
