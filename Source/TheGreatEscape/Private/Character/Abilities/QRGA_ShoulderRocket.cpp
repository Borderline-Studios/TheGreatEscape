// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/QRGA_ShoulderRocket.h"

#include "EnemyRework.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_ShoulderRocket::UQRGA_ShoulderRocket()
{
	AbilityInputID = EGASAbilityInputID::QAbility;
}

void UQRGA_ShoulderRocket::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	InputRelaese = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputRelaese->OnRelease.AddDynamic(this, &UQRGA_ShoulderRocket::ReleasedInput);
	InputRelaese->ReadyForActivation();
	GetPlayerReference()->ActivateRocket();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyRework::StaticClass(), FoundActors);
	ClosestActor = UGameplayStatics::FindNearestActor(GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(), FoundActors, RocketDistance);
	if(GetPlayerReference()->GetController()->LineOfSightTo(ClosestActor, GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(), false))
	{
		bActorFound = true;
		GetPlayerReference()->RocketTargetWidget(ClosestActor);
	}
	else
	{
		bActorFound = false;
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

}

void UQRGA_ShoulderRocket::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_ShoulderRocket::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_ShoulderRocket::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_ShoulderRocket::ReleasedInput(float TimePressed)
{
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle ,this,&UQRGA_ShoulderRocket::CallEndAbility, CooldownTime, false);
	AEnemyRework* EnemyCast = Cast<AEnemyRework>(ClosestActor);
	EnemyCast->PostDeathProcess();
	EnemyCast->Destroy();
	GetPlayerReference()->RemoveRocketTargetWidget();
	GetPlayerReference()->DeactivateRocket();
}

void UQRGA_ShoulderRocket::CallEndAbility()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
