// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Tests/QRGA_ADSTest.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_ADSTest::UQRGA_ADSTest()
{
	AbilityInputID =  EGASAbilityInputID::AimDownSights;
}

void UQRGA_ADSTest::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Held"));
	
	UAbilityTask_WaitInputRelease* Task_WaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	Task_WaitInputRelease->OnRelease.AddDynamic(this, &UQRGA_ADSTest::CallEndAbility);
	Task_WaitInputRelease->ReadyForActivation();
	
}

void UQRGA_ADSTest::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

bool UQRGA_ADSTest::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_ADSTest::GetPlayerReference()
{

	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_ADSTest::CallEndAbility(float TimeHeld)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
