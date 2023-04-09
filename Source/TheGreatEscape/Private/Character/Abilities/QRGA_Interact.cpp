// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Interact.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TrainControlls.h"
#include "TrainStopButton.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interactables/BatteryInteractable.h"
#include "Interactables/TrainHorn.h"
#include "Objectives/ObjectiveGate.h"

UQRGA_Interact::UQRGA_Interact()
{
	//Setting the input key via enum
	AbilityInputID = EGASAbilityInputID::Interact;
}

void UQRGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Creating variables for the Line Trace
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPlayerReferance());
	//Performs line trace from the middle of players view and checks if anything was hit
	if (GetWorld()->LineTraceSingleByChannel(HitResult,GetPlayerReferance()->GetFirstPersonCameraComponent()->GetComponentLocation(),
												   GetPlayerReferance()->GetFirstPersonCameraComponent()->GetComponentLocation() +
													   GetPlayerReferance()->GetFirstPersonCameraComponent()->GetForwardVector() * 1000,
													   ECC_Visibility, Params))
	{
		if (IsValid(HitResult.GetActor()) && IsValid(HitResult.GetComponent()))
		{
			//Checks if the hit actor was an interactable object (uses unreals tag system)
			if(HitResult.GetActor()->ActorHasTag("Interactable"))
			{
				/**********************************************************************************************************/
				// cpp class specific behaviour goes here.

				//Checks if its the Train controls
				if (ATrainControlls* ControlsRef = Cast<ATrainControlls>(HitResult.GetActor()))
				{
					ControlsRef->UpdateEngineSpeed();
					// play sound
					UGameplayStatics::PlaySoundAtLocation(GetWorld(),GetPlayerReferance()->LeverSFX[FMath::RandRange(0,2)], ControlsRef->GetActorLocation(), FRotator(0,0,0), 1.0f);
				}
				else if (HitResult.GetActor()->ActorHasTag("Horn"))
				{
						ATrainHorn* TrainHorn = Cast<ATrainHorn>(HitResult.GetActor());
						TrainHorn->PlayHorn();
				}
				//Check if its the train stop button
				else if (ATrainStopButton* ButtonRef = Cast<ATrainStopButton>(HitResult.GetActor()))
				{
					ButtonRef->ToggleTrainMovement();

					// play sound
					UGameplayStatics::PlaySoundAtLocation(GetWorld(),GetPlayerReferance()->ButtonSFX, ButtonRef->GetActorLocation(), FRotator(0,0,0), 1.0f);

				}
				else if (HitResult.GetActor()->ActorHasTag("MedKit"))
				{
					FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(HealingEffectClass);

					GetPlayerReferance()->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(),GetPlayerReferance()->GetAbilitySystemComponent());
					bool Found;
					float Health = GetPlayerReferance()->GetAbilitySystemComponent()->GetGameplayAttributeValue(GetPlayerReferance()->Attributes->GetHealthAttribute(), Found);
					if (Health > 30.0f && Found)
					{
						GetPlayerReferance()->DisableVignette();
					}
				}
				else if (HitResult.GetActor()->ActorHasTag("Rifle"))
				{
					HitResult.GetActor()->Destroy();
					GetPlayerReferance()->bRiflePickedUp = true;
				}
				
				//Checks if its a pick upable
				//TODO Make this better (Pick up and drop)
				else
				{
					if (AObjectiveGate* ParentGate = Cast<AObjectiveGate>(HitResult.GetActor()->GetAttachParentActor()))
					{
						ParentGate->UpdateObjectiveText("Take this back to the gate blocking the train!");
					}
					
					HitResult.GetActor()->AttachToActor(GetPlayerReferance(), FAttachmentTransformRules::SnapToTargetNotIncludingScale , NAME_None);
					HitResult.GetComponent()->SetVisibility(false, false);
					GetPlayerReferance()->bBatteryPickedUp = true;
					
				}
				//ends the ability
				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
			}
			//TODO test if this actually does anything.
			//Checks if the component is Interactable
			else if (HitResult.GetComponent()->ComponentHasTag("Interactable"))
			{
				//Disables collisions
				HitResult.GetComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
				//Attaches to player
				HitResult.GetComponent()->AttachToComponent(GetPlayerReferance()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale , NAME_None);
				HitResult.GetComponent()->SetVisibility(false, false);

			}
			else
			{
				//Ends ability if not true
				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
			}

		}
		
	}
	//Ends ability no matter what (avoid lock up)
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
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}
