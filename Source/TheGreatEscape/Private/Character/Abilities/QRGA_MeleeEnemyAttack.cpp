// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRGA_MeleeEnemyAttack.h
// Description : Gameplay ability to have melee enemy attack
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "Character/Abilities/QRGA_MeleeEnemyAttack.h"

#include "AbilitySystemBlueprintLibrary.h"

UQRGA_MeleeEnemyAttack::UQRGA_MeleeEnemyAttack()
{
}

void UQRGA_MeleeEnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, GetEnemyRef()->GetActorLocation(), GetEnemyRef()->GetActorLocation() +
		GetEnemyRef()->GetActorForwardVector() * 5000, ECC_Visibility))
	{
		UE_LOG(LogTemp, Warning, TEXT("atac player boi"));
		// attac
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if(ASC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage player boi %s"), *HitResult.GetActor()->GetName());
			FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
			ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
		}
	}
	
}

void UQRGA_MeleeEnemyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_MeleeEnemyAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

AEnemyRework* UQRGA_MeleeEnemyAttack::GetEnemyRef()
{
	AEnemyRework* Enemy = Cast<AEnemyRework>(GetAvatarActorFromActorInfo());
	return Enemy;
}
