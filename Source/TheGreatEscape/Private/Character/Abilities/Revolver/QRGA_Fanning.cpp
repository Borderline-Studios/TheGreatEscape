// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Revolver/QRGA_Fanning.h"

#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_Fanning::UQRGA_Fanning()
{
	AbilityInputID = EGASAbilityInputID::AimDownSights;
}

void UQRGA_Fanning::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ShotsRemaining = GetPlayerReference()->PlayerAmmo;

	GetWorld()->GetTimerManager().SetTimer(FanHandle,this ,&UQRGA_Fanning::FanLoop, 0.2f, true);
}

void UQRGA_Fanning::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetWorld()->GetTimerManager().ClearTimer(FanHandle);
}

bool UQRGA_Fanning::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_Fanning::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_Fanning::FanLoop()
{
	ShotsRemaining--;
	if (GetPlayerReference()->PlayerAmmo != 0)
	{
		GetPlayerReference()->PlayerAmmo--;
	}
	GetPlayerReference()->Mesh1P->GetAnimInstance()->Montage_JumpToSection("Fire");
	//Added dynamic notify and triggers function if notify is received
	GetPlayerReference()->Mesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Fanning::CallEndAbility);
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	//Plays the sound at the player
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX[FMath::RandRange(0,3)], CamCompLocation,FRotator(0,0,0), 0.3, FMath::RandRange(1.0f, 1.0f));
	if (ShotsRemaining == 0)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UQRGA_Fanning::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//Checks if the Nofity name
	if (NotifyName == FName("FinishedFire"))
	{
		if(ShotsRemaining <= 0)
		{
			//Ends ability is the animation is done
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
	}
}
