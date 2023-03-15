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

/**
 * @brief Constructor
 */
UQRGA_MeleeEnemyAttack::UQRGA_MeleeEnemyAttack()
{
	// Constructor
}

/**
 * @brief When the ability is activated this function is called
 * @param Handle Gameplay Ability Spec Handle
 * @param ActorInfo Info of the actor
 * @param ActivationInfo Information about the activation
 * @param TriggerEventData Gameplay event data
 */
void UQRGA_MeleeEnemyAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	

	FVector start = GetEnemyRef()->GetActorLocation();
	FVector end = start + GetEnemyRef()->GetActorForwardVector() * LineTraceMultiplier;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetEnemyRef());
	
	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, Params))
	{
		//UE_LOG(LogTemp, Warning, TEXT("atac player boi"));
		// attac
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if(ASC)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Damage player boi %s"), *HitResult.GetActor()->GetName());
			FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
			ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
		}
	}
	//DrawDebugLine(GetWorld(), start, end, FColor::Purple, false, 5.0f, 0, 5.0f);
	GetEnemyRef()->bAttacking = false;
}

/**
 * @brief Called when ability is ended
 * @param Handle gameplay ability spec handle
 * @param ActorInfo Info about the actor
 * @param ActivationInfo Info of the activation
 * @param bReplicateEndAbility bool to replicated the end ability
 * @param bWasCancelled bool to check if it was cancelled
 */
void UQRGA_MeleeEnemyAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

/**
 * @brief checks if you can activate ability
 * @param Handle gameplay ability spec handle
 * @param ActorInfo info about the actor
 * @param SourceTags gameplay tag counter
 * @param TargetTags target gameplay tag
 * @param OptionalRelevantTags gameplay tag
 * @return bool if it can activate
 */
bool UQRGA_MeleeEnemyAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

/**
 * @brief get a ref to the enemy 
 * @return the enemy that had the ability
 */
AEnemyRework* UQRGA_MeleeEnemyAttack::GetEnemyRef()
{
	AEnemyRework* Enemy = Cast<AEnemyRework>(GetAvatarActorFromActorInfo());
	return Enemy;
}
