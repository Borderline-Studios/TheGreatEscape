// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Interact.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Elevator.h"
#include "TrainControlls.h"
#include "TrainStopButton.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interactables/BatteryInteractable.h"
#include "Interactables/InteractableDoor.h"
#include "Interactables/TrainHorn.h"
#include "Objectives/DestroyableTarget.h"
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

	GetWorld()->GetTimerManager().SetTimer(ForceEndHandle, this ,&UQRGA_Interact::ForceEndAbility, 1.0f, false);
	
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
			if (AElevator* Elevator = Cast<AElevator>(HitResult.GetActor()))
			{
				Elevator->Activate();
			}
			//Checks if the hit actor was an interactable object (uses unreals tag system)
			if(HitResult.GetActor()->ActorHasTag("Interactable"))
			{
				/**********************************************************************************************************/
				// cpp class specific behaviour goes here.

				//Checks if its the Train controls
				if (ATrainControlls* ControlsRef = Cast<ATrainControlls>(HitResult.GetActor()))
				{
					ControlsRef->ControlsInteraction();
				}
				else if (HitResult.GetActor()->ActorHasTag("Horn"))
				{
					ATrainHorn* TrainHorn = Cast<ATrainHorn>(HitResult.GetActor());
					TrainHorn->PlayHorn();
				}
				else if (HitResult.GetActor()->ActorHasTag("RoboPal"))
				{
					HitResult.GetActor()->Destroy();
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
					bool Found;
					float Health = GetPlayerReferance()->GetAbilitySystemComponent()->GetGameplayAttributeValue(GetPlayerReferance()->Attributes->GetHealthAttribute(), Found);
					bool MHFound;
					float MaxHealth = GetPlayerReferance()->GetAbilitySystemComponent()->GetGameplayAttributeValue(GetPlayerReferance()->Attributes->GetMaxHealthAttribute(),MHFound);
					FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(HealingEffectClass);

					if (Health < MaxHealth)
					{
						GetPlayerReferance()->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(),GetPlayerReferance()->GetAbilitySystemComponent());
						if (!HealSFX.IsEmpty())
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), HealSFX[0], HitResult.Location);
						}
					}
					else if (Health >= MaxHealth)
					{
						if (!FullHealSFX.IsEmpty())
						{
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), FullHealSFX[0], HitResult.Location);
						}
					}
					if (Health > 30.0f && Found)
					{
						GetPlayerReferance()->DisableVignette();
					}
					GetPlayerReferance()->PostHitProcess();
				}
				else if (HitResult.GetActor()->ActorHasTag("Door"))
				{
					if (AInteractableDoor* IDoor = Cast<AInteractableDoor>(HitResult.GetActor()))
					{
						IDoor->OpenDoor();
					}
				}
				else if (HitResult.GetActor()->ActorHasTag("Rifle"))
				{
					GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("Deactivate");
					//Checks for an Animnotify then triggers function
					GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Interact::CallEndAbility);
					ActorToDestory = HitResult.GetActor();
				}
				else if (HitResult.GetActor()->ActorHasTag("Revolver"))
				{
					ActorToDestory = HitResult.GetActor();
					FirstRevovlerEqiup();
					GetPlayerReferance()->ApplyUI();
					ActorToDestory->Destroy();
				}
				//Checks if its a pick upable
				//TODO Make this better (Pick up and drop)
				else	// Checking for the battery
				{
					if (AObjectiveGate* GateRef = Cast<AObjectiveGate>(UGameplayStatics::GetActorOfClass(this, AObjectiveGate::StaticClass())))
					{
						GateRef->RepopulateBatteryObjectiveText();
					}
					HitResult.GetActor()->Destroy();
					GetPlayerReferance()->IncrementBatteryCount();
				}
				//ends the ability
				//EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
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
	//EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
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

void UQRGA_Interact::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//Checks if the Nofity name
	if (NotifyName == FName("Gather"))
	{
		GetPlayerReferance()->RevolverMesh1P->SetVisibility(false);
		GetPlayerReferance()->bRiflePickedUp = true;
		ActorToDestory->Destroy();
		GetPlayerReferance()->RifleMesh1P->SetVisibility(true);
		GetPlayerReferance()->bRevolverEquipped = false;
		GetPlayerReferance()->bRifleEquipped = true;
		FirstRifleEqiup();
	}
	if (NotifyName == FName("Activated"))
	{
		//Ends ability is the animation is done
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	if (NotifyName == FName("InspectFinished"))
	{
		//Ends ability is the animation is done
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	if (NotifyName == FName("FinishedInspect"))
	{
		GetPlayerReferance()->bRevolverEquipped = true;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("RevolverEquiped"));
		//Ends ability is the animation is done
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UQRGA_Interact::ForceEndAbility()
{
	//Ends ability is the animation is done
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_Interact::FirstRifleEqiup()
{
	GetPlayerReferance()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("Inspect");
	GetPlayerReferance()->RifleMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_Interact::CallEndAbility);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpRifleSFX, GetPlayerReferance()->GetActorLocation());
}

void UQRGA_Interact::FirstRevovlerEqiup()
{
	GetPlayerReferance()->PickUpRobopal();
	GetPlayerReferance()->bRevolverEquipped = true;
	GetPlayerReferance()->RevolverMesh1P->SetVisibility(true);
	GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("Inspect");
	GetPlayerReferance()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_Interact::CallEndAbility);
}
