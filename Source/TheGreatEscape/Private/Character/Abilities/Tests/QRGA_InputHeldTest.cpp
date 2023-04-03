// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Tests/QRGA_InputHeldTest.h"

#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "Math/UnitConversion.h"

UQRGA_InputHeldTest::UQRGA_InputHeldTest()
{
	AbilityInputID = EGASAbilityInputID::QAbility;
	
}

void UQRGA_InputHeldTest::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	InputRelaese = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRelaese->OnRelease.AddDynamic(this, &UQRGA_InputHeldTest::ReleasedInput);
	InputRelaese->ReadyForActivation();

	AbilityLogic();
}

void UQRGA_InputHeldTest::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_InputHeldTest::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UQRGA_InputHeldTest::ReleasedInput(float TimePressed)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Released"));
	//End ability if ammo is full (Reload no needed)
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_InputHeldTest::AbilityLogic()
{
	UAbilityTask_WaitDelay* WaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, TimeDelay);
	WaitDelay->OnFinish.AddDynamic(this, &UQRGA_InputHeldTest::OtherStuff);
	WaitDelay->ReadyForActivation();
	
	
}

void UQRGA_InputHeldTest::OtherStuff()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.48f, FColor::Green, TEXT("Still Held"));
}
