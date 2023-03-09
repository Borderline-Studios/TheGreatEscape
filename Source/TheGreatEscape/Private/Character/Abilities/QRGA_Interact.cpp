// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Interact.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TrainControlls.h"
#include "TrainStopButton.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interactables/BatteryInteractable.h"

UQRGA_Interact::UQRGA_Interact()
{
	AbilityInputID = EGASAbilityInputID::Interact;
}

void UQRGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPlayerReferance());
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Interact acti!"));	
	if (GetWorld()->LineTraceSingleByChannel(HitResult,GetPlayerReferance()->GetFirstPersonCameraComponent()->GetComponentLocation(),
												   GetPlayerReferance()->GetFirstPersonCameraComponent()->GetComponentLocation() +
													   GetPlayerReferance()->GetFirstPersonCameraComponent()->GetForwardVector() * 1000,
													   ECC_Visibility, Params))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Something Hit!"));
		
		if(HitResult.GetActor()->ActorHasTag("Interactable"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Interactable!"));
			/**************************************************************************************************************************************************************************/
			// cpp class specific behaviour goes here.
			
			if(ABatteryInteractable* BatteryInteractableRef = Cast<ABatteryInteractable>(HitResult.GetActor()))
			{
				HitResult.GetActor()->SetActorEnableCollision(false);
				HitResult.GetActor()->AttachToActor(GetPlayerReferance(), FAttachmentTransformRules::SnapToTargetNotIncludingScale , NAME_None);

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Picked Up!"));
				BatteryInteractableRef->SetPickedUp();
			}
			else if (ATrainControlls* ControlsRef = Cast<ATrainControlls>(HitResult.GetActor()))
			{
				ControlsRef->UpdateEngineSpeed();
			}
			else if (ATrainStopButton* ButtonRef = Cast<ATrainStopButton>(HitResult.GetActor()))
			{
				ButtonRef->ToggleTrainMovement();
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Not Picked Up!"));
			}
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not Interactable!"));
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
	}
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UQRGA_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_Interact::GetPlayerReferance()
{
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	return CharacterRef;
}
