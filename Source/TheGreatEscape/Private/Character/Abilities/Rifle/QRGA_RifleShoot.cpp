// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Rifle/QRGA_RifleShoot.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnemyRework.h"
#include "EnemyReworkDrone.h"
#include "EnemyReworkHybrid.h"
#include "NiagaraFunctionLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Camera/CameraComponent.h"
#include "Character/Abilities/QRGA_Crouch.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interactables/ObjectiveShield.h"
#include "Kismet/KismetMathLibrary.h"

UQRGA_RifleShoot::UQRGA_RifleShoot()
{
	AbilityInputID = EGASAbilityInputID::Shoot;
}

void UQRGA_RifleShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if(GetPlayerReference()->bRifleEquipped)
	{
		InputRelaese = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
		InputRelaese->OnRelease.AddDynamic(this, &UQRGA_RifleShoot::ReleasedInput);
		InputRelaese->ReadyForActivation();
		FireLoop();
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

}

void UQRGA_RifleShoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

bool UQRGA_RifleShoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_RifleShoot::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_RifleShoot::ReleasedInput(float TimePressed)
{
	bInputReleased = true;
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_RifleShoot::FireLoop()
{
	if (GetPlayerReference()->CurrentRifleAmmo > 0)
	{
		GetPlayerReference()->CurrentRifleAmmo--;
		UGameplayStatics::PlaySoundAtLocation(this, ShootSFX[FMath::RandRange(0,3)], GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation(),
												1, 1, 0);
		if (GetPlayerReference()->bADS)
		{
			GetPlayerReference()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("AimFire");
			GetPlayerReference()->RifleMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RifleShoot::CallEndAbility);
		}
		else
		{
			GetPlayerReference()->RifleMesh1P->GetAnimInstance()->Montage_JumpToSection("Fire");
			GetPlayerReference()->RifleMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UQRGA_RifleShoot::CallEndAbility);
		}
		FHitResult HitResult = HitScan(GetPlayerReference()->MaxShotRange);
		ActivateEffects(HitResult);
		if (HitResult.IsValidBlockingHit())
		{
			HitEnemyCheck(HitResult);
		}
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}

}

void UQRGA_RifleShoot::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName("FireFinished"))
	{
		if (GetPlayerReference()->CurrentRifleAmmo <= 0)
		{
			//Ends ability is the animation is done and ammo is out
			EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		}
		else if(GetPlayerReference()->CurrentRifleAmmo > 0)
		{
			FireLoop();
		}
	}
}

FHitResult UQRGA_RifleShoot::HitScan(float MaxDistance)
{
	FCollisionQueryParams Params;
	//Ignore self
	Params.AddIgnoredActor(GetPlayerReference());
	FHitResult HitScanResult;
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	FVector CamCompLocationWithDeviation = FVector(CamCompLocation.X,CamCompLocation.Y + FMath::FRandRange(-75.0f, 75.0f), CamCompLocation.Z + FMath::FRandRange(-75.0f, 75.0f));
	GetWorld()->LineTraceSingleByChannel(HitScanResult,CamCompLocation,CamCompLocationWithDeviation + CamCompForwardVector * MaxDistance,ECC_Visibility, Params);
	//DrawDebugLine(GetWorld(), CamCompLocation, CamCompLocationWithDeviation + CamCompForwardVector * MaxDistance, FColor::Red,false, 1.0f , 0, 5.0f );
	return HitScanResult;
}

void UQRGA_RifleShoot::ActivateEffects(FHitResult HitInput)
{
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	int MaxShotRange = GetPlayerReference()->MaxShotRange;
	
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX[FMath::RandRange(0,3)], CamCompLocation,FRotator(0,0,0), 0.3, FMath::RandRange(0.9,1.1));
	FVector MuzzleLocation = GetPlayerReference()->RifleMuzzleSphere->GetComponentLocation();
	FRotator MuzzleRotRef = GetPlayerReference()->RifleMuzzleSphere->GetComponentRotation();
	FRotator MuzzleRotation = FRotator(MuzzleRotRef.Pitch, MuzzleRotRef.Yaw + 90, MuzzleRotRef.Roll - 90.0f);
	UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleVFX, GetPlayerReference()->RifleMuzzleSphere, FName(GetPlayerReference()->RifleMuzzleSphere->GetName()),
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

void UQRGA_RifleShoot::HitEnemyCheck(FHitResult HitInput)
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
			
			//play animation
			if (AEnemyRework* Enemy = Cast<AEnemyRework>(HitInput.GetActor()))
			{
				if (AEnemyReworkDrone* enemyDrone = Cast<AEnemyReworkDrone>(Enemy))
				{
	
				}
				else if (AEnemyReworkHybrid* enemyHybrid= Cast<AEnemyReworkHybrid>(Enemy))
				{
					// hybrid tins
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
			bool bFound;
			float Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetShieldAttribute(), bFound);
			if (Value > 0 && bFound)
			{
				//Uses the out going handle to deal damage
				ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
				GetPlayerReference()->CreateDamageWidget(HitInput, 10.0f, true);
			}
			else
			{
				GetPlayerReference()->CreateDamageWidget(HitInput, 0.0f, true);
			}
		}
		else if (AObjectiveShield* ObjectiveShield = Cast<AObjectiveShield>(HitInput.GetActor()))
		{
			ObjectiveShield->PostHitProcess();
		}
	}
}
