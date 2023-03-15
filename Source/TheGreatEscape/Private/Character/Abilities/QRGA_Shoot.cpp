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
	AbilityInputID = EGASAbilityInputID::Shoot;
}

void UQRGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if(GetPlayerReference()->PlayerAmmo <= 0)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else
	{
		GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Fire");
		GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Shoot::CallEndAbility);
		GetPlayerReference()->PlayerAmmo--;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX,
											  GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
											  FRotator(0,0,0), 0.3, 1);
		
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetPlayerReference());
		if (GetWorld()->LineTraceSingleByChannel(HitResult,GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
													   GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation() +
														   GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector() * 20000,
														   ECC_Visibility, Params))
		{
			GetPlayerReference()->GetController()->SetControlRotation(FRotator(GetPlayerReference()->GetController()->GetControlRotation().Pitch + 2.1,
																					 GetPlayerReference()->GetController()->GetControlRotation().Yaw,
																					 GetPlayerReference()->GetController()->GetControlRotation().Roll));

			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(GetPlayerReference()->CamShake, 1.0f);
			
			
			UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
			if(ASC)
			{
				FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
				ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
			}
		}
	}
	
	//EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//GetWorld()->GetTimerManager().ClearTimer(ShootHandle);
}

void UQRGA_Shoot::ToggleShooting()
{
}


APlayerCharacter* UQRGA_Shoot::GetPlayerReference()
{
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	return CharacterRef;
}

void UQRGA_Shoot::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("FinishedFire"))
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
