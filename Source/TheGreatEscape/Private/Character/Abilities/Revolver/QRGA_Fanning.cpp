// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Revolver/QRGA_Fanning.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Boss.h"
#include "EnemyRework.h"
#include "EnemyReworkDrone.h"
#include "EnemyReworkHybrid.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interactables/WorldInteractTrigger.h"
#include "Kismet/KismetMathLibrary.h"

UQRGA_Fanning::UQRGA_Fanning()
{
	AbilityInputID = EGASAbilityInputID::AimDownSights;
}

void UQRGA_Fanning::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (GetPlayerReference()->bRevolverEquipped)
	{
		GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("FanActivate");
		//Added dynamic notify and triggers function if notify is received
		GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_Fanning::CallEndAbility);
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

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
	GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection(FanSections[FMath::RandRange(0,2)]);
	//Added dynamic notify and triggers function if notify is received
	GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_Fanning::CallEndAbility);
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	//Plays the sound at the player
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX[FMath::RandRange(0,3)], CamCompLocation,FRotator(0,0,0), 0.2, FMath::RandRange(1.0f, 1.0f));
	FHitResult HitResult = HitScan(GetPlayerReference()->MaxShotRange);
	ActivateTraceParticle(HitResult);
	ActivateEffects();
	if (HitResult.IsValidBlockingHit())
	{
		HitEnemyCheck(HitResult);
		//Here HitTagCheck
		HitTagCheck(HitResult);
	}
}

void UQRGA_Fanning::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("FanActiFinished"))
	{
		if (GetPlayerReference()->PlayerAmmo > 0)
		{
			ShotsRemaining = GetPlayerReference()->PlayerAmmo;

			FanLoop();
		}
		else
		{
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
	}
	//Checks if the Nofity name
	if (NotifyName == FName("AnimFinished"))
	{
		if(ShotsRemaining <= 0)
		{
			EndFanning();
		}
		else if(ShotsRemaining > 0)
		{
			FanLoop();
		}
	}
	if (NotifyName == FName("FanDeactiFinished"))
	{
		//Ends ability is the animation is done
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UQRGA_Fanning::EndFanning()
{
	GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("FanDeactivate");
	//Added dynamic notify and triggers function if notify is received
	GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_Fanning::CallEndAbility);	
}

void UQRGA_Fanning::ActivateEffects()
{
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	int MaxShotRange = GetPlayerReference()->MaxShotRange;
	
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX[FMath::RandRange(0,3)], CamCompLocation,FRotator(0,0,0), 0.3, FMath::RandRange(0.9,1.1));
	FVector MuzzleLocation = GetPlayerReference()->MuzzleSphere->GetComponentLocation();
	FRotator MuzzleRotRef = GetPlayerReference()->MuzzleSphere->GetComponentRotation();
	FRotator MuzzleRotation = FRotator(MuzzleRotRef.Pitch, MuzzleRotRef.Yaw, MuzzleRotRef.Roll);
	UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleVFX, GetPlayerReference()->MuzzleSphere, FName(GetPlayerReference()->MuzzleSphere->GetName()),
													MuzzleLocation, MuzzleRotation, EAttachLocation::KeepWorldPosition, false, true);

	float CamControlPitch = GetPlayerReference()->GetController()->GetControlRotation().Pitch;
	float CamControlYaw = GetPlayerReference()->GetController()->GetControlRotation().Yaw;
	float CamControlRoll = GetPlayerReference()->GetController()->GetControlRotation().Roll;
	float AimPunchAmount = 2.1;
				
	//Add crosshair recoil (aim punch)
	GetPlayerReference()->GetController()->SetControlRotation(FRotator(CamControlPitch + AimPunchAmount, CamControlYaw, CamControlRoll));
	//Cam Shake
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(GetPlayerReference()->CamShake, 1.0f);
}

FHitResult UQRGA_Fanning::HitScan(float MaxDistance)
{
	FCollisionQueryParams Params;
	//Ignore self
	Params.AddIgnoredActor(GetPlayerReference());
	FHitResult HitScanResult;
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	FVector CamCompLocationWithDeviation = FVector(CamCompLocation.X,CamCompLocation.Y + FMath::FRandRange(-400.0f, 400.0f), CamCompLocation.Z + FMath::FRandRange(-400.0f, 400.0f));
	GetWorld()->LineTraceSingleByChannel(HitScanResult,CamCompLocation,CamCompLocationWithDeviation + CamCompForwardVector * MaxDistance,ECC_Visibility, Params);
	//DrawDebugLine(GetWorld(), CamCompLocation, CamCompLocationWithDeviation + CamCompForwardVector * MaxDistance, FColor::Red,false, 1.0f , 0, 5.0f );
	return HitScanResult;
}

void UQRGA_Fanning::HitEnemyCheck(FHitResult HitInput)
{
	if (HitInput.GetActor())
	{
		//Getting the ability system component from the hit actor
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitInput.GetActor());
		//Check if ASC is vaild
		if(ASC)
		{
			//Creates damage effect outgoing handle
			FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
							
			// bool to check if it was found & the value the health equals
			bool bFound;
			float Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetShieldAttribute(), bFound);
			if (Value <= 0 && bFound)
			{
				//Uses the out going handle to deal damage
				ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
				GetPlayerReference()->CreateDamageWidget(HitInput, 10.0f, false);
			}
			else if (!bFound)
			{
				//Uses the out going handle to deal damage
				ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
				GetPlayerReference()->CreateDamageWidget(HitInput, 10.0f, false);
			}
			else
			{
				GetPlayerReference()->CreateDamageWidget(HitInput, 0.0f, false);
			}

			
			//Actiavte hit VFX on hit object/actor
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, HitInput.Location, HitInput.GetActor()->GetActorRotation());
			//play animation
			if (AEnemyRework* Enemy = Cast<AEnemyRework>(HitInput.GetActor()))
			{
				if (AEnemyReworkDrone* enemyDrone = Cast<AEnemyReworkDrone>(Enemy))
				{
					enemyDrone->PostHitProcess();
				}
				else if (AEnemyReworkHybrid* enemyHybrid= Cast<AEnemyReworkHybrid>(Enemy))
				{
					// hybrid tins
					enemyHybrid->GetMesh()->GetAnimInstance()->Montage_JumpToSection("Hit");
					// audio
					// hybrid tins
					enemyHybrid->PostHitProcess();
				}
				else
				{
					Enemy->GetMesh()->GetAnimInstance()->Montage_JumpToSection("Hit");
	
					// ToDo Maybe put this into function as I (Toni) copied it
					if(UKismetMathLibrary::RandomBoolWithWeight(0.9) && Enemy->SFXTiggerNum == 0)
					{
						Enemy->SFXTiggerNum = FMath::RandRange(4,7 );
						int RandomSFX = FMath::RandRange(0,2 );
						UGameplayStatics::PlaySoundAtLocation(GetWorld(),Enemy->EnemyHitSFX[RandomSFX], Enemy->GetActorLocation(), FRotator(0,0,0), 0.4f);
					}
					else
					{
						Enemy->SFXTiggerNum--;
					}
	
					Enemy->PostHitProcess();
				}
	
				if(UKismetMathLibrary::RandomBoolWithWeight(0.9) && GetPlayerReference()->VoiceLineTiggerNum == 0)
				{
					if(GetPlayerReference()->QuipSFX.IsEmpty())
					{
						GetPlayerReference()->VoiceLineTiggerNum = 6;
						int RandomSFX = FMath::RandRange(0,4 );
						FVector CamComLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetPlayerReference()->QuipSFX[RandomSFX], CamComLocation, FRotator(0,0,0), 1.0);
					}
				}
				else
				{
					GetPlayerReference()->VoiceLineTiggerNum--;
				}
			}
			else if (ABoss* Boss = Cast<ABoss>(HitInput.GetActor()))
			{
				Boss->PostHitProcess();
			}
		}
	}
}

void UQRGA_Fanning::HitTagCheck(FHitResult HitInput)
{
	if(HitInput.IsValidBlockingHit())
	{
		if (HitInput.GetActor())
		{
			if (HitInput.GetActor()->ActorHasTag("Trigger"))
			{
				if (HitInput.GetActor()->ActorHasTag("Bridge"))
				{
					if (AWorldInteractTrigger* WorldInteractTrigger = Cast<AWorldInteractTrigger>(HitInput.GetActor()))
					{
						if (HitInput.GetActor() == WorldInteractTrigger)
						{
							WorldInteractTrigger->MoveActorOnTrigger(); 
						}
					}
				}
			}
		}
	}
}
