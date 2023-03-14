// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/QRGA_ResourceGather.h"

#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_ResourceGather::UQRGA_ResourceGather()
{
	AbilityInputID = EGASAbilityInputID::Gather;
}

void UQRGA_ResourceGather::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Deactivate");
	GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_ResourceGather::ResourceLogic);

}

void UQRGA_ResourceGather::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.Clear();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_ResourceGather::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UQRGA_ResourceGather::ResourceLogic(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if(NotifyName == FName("Gather"))
	{
		FHitResult HitResult;
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(GetPlayerReference());

		if (GetWorld()->LineTraceSingleByChannel(HitResult,GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
														   GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation() +
															   GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector() * 1000,
															   ECC_Visibility, Params))
		{
			if (HitResult.GetActor())
			{
				if(HitResult.GetActor()->ActorHasTag("Resource"))
				{
					if(HitResult.GetActor()->ActorHasTag("Scraps"))
						GetPlayerReference()->NumScrap = GetPlayerReference()->NumScrap + 3;
					HitResult.GetActor()->Destroy();
				}
			}
			else
				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
			
		}

		GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Activate");
		GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_ResourceGather::CallEndAbilitity);
	}
}

void UQRGA_ResourceGather::CallEndAbilitity(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if(NotifyName == FName("FinishedGather"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

APlayerCharacter* UQRGA_ResourceGather::GetPlayerReference()
{
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	return CharacterRef;
}
