// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Reload.h"

#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_Reload::UQRGA_Reload()
{
	AbilityInputID = EGASAbilityInputID::Reload;
}

void UQRGA_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GetPlayerReference()->PlayerAmmo = 0;
	GetPlayerReference()->PlayerAmmo = 6;

	GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Reload");
	GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Reload::CallEndAbility);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSFX ,
										  GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
										  FRotator(0,0,0), 0.3, 1);
		
	
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
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	return CharacterRef;
}

void UQRGA_Reload::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("FinishedReload"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	
}
