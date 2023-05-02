// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

//TODO see if this ability can be intergreated with interactables (or if this current scope it is even nessessary)
#include "Character/Abilities/QRGA_ResourceGather.h"

#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"

UQRGA_ResourceGather::UQRGA_ResourceGather()
{
	//Setting the input via the enum
	AbilityInputID = EGASAbilityInputID::Gather;
}

void UQRGA_ResourceGather::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//jumps the animontage to the Deactivate section
	GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("Deactivate");
	//Addes notify to actiave function 
	GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_ResourceGather::ResourceLogic);

}

void UQRGA_ResourceGather::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
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
	//Checks notify name
	if(NotifyName == FName("Gather"))
	{
		//Variables for Line trace
		FHitResult HitResult;
		FCollisionQueryParams Params;

		//ignore self
		Params.AddIgnoredActor(GetPlayerReference());

		//Check if linecraft hits anything
		if (GetWorld()->LineTraceSingleByChannel(HitResult,GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
														   GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation() +
															   GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector() * 1000,
															   ECC_Visibility, Params))
		{
			//Checks if its an actor
			if (HitResult.GetActor())
			{
				//Checks if the actor has the tag recourse
				if(HitResult.GetActor()->ActorHasTag("Resource"))
				{
					//check if the actor has the tag scraps
					if(HitResult.GetActor()->ActorHasTag("Scraps"))
						//increase players number of scraps
						GetPlayerReference()->NumScrap = GetPlayerReference()->NumScrap + 3;
					HitResult.GetActor()->Destroy();
					//destroys the actor to stop infinite resources
				}
			}
			else
				//ends the ability
				//TODO check if this breaks the stuff
				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
			
		}
		//Jumps animontage to activate
		GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("Activate");
		//Add notify to trigger ability when done
		GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_ResourceGather::CallEndAbilitity);
	}
}

void UQRGA_ResourceGather::CallEndAbilitity(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//Checks notify name
	if(NotifyName == FName("FinishedGather"))
	{
		//Ends ability
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

APlayerCharacter* UQRGA_ResourceGather::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}
