// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/QRGA_Shoot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BlueprintNodeHelpers.h"
#include "ShaderCompiler.h"
//#include "../../../../../../../../../../../../Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/INCLUDE/string"
#include "EnemyRework.h"
#include "EnemyReworkDrone.h"
#include "EnemyReworkHybrid.h"
#include "Camera/CameraComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Chaos/ChaosPerfTest.h"
#include "Kismet/GameplayStatics.h"

#include "Character/QRCharacter.h"
#include "Character/BASE/GASBASECharacter.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Interactables/WorldInteractTrigger.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviourTree/Utils.h"
#include "Perception/AISense_Hearing.h"


UQRGA_Shoot::UQRGA_Shoot()
{
	//sets the input key via the Enum
	AbilityInputID = EGASAbilityInputID::Shoot;
}

void UQRGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	int MaxShotRange = GetPlayerReference()->MaxShotRange;
	//Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//Checks is ammo is depleated
	if(GetPlayerReference()->PlayerAmmo <= 0)
	{
		//Plays the sound at the player
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptySFX[FMath::RandRange(0,3)], CamCompLocation,FRotator(0,0,0), 0.3, FMath::RandRange(0.7,0.9));
		//If ammo is 0 end ability
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else if (!GetPlayerReference()->bTransADS)
	{
		//Decrements the Ammo
		GetPlayerReference()->PlayerAmmo--;

		if (GetPlayerReference()->bADS)
		{
			//Jumps the animontage to the fire section
			GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("FireADS");
			//Added dynamic notify and triggers function if notify is received
			GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Shoot::CallEndAbility);
		}
		else
		{
		
			//Jumps the animontage to the fire section
			GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("Fire");
			//Added dynamic notify and triggers function if notify is received
			GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_Shoot::CallEndAbility);
		}

		ActivateEffects();
		FHitResult HitResult = HitScan(GetPlayerReference()->MaxShotRange);
		if (HitResult.IsValidBlockingHit())
		{
			HitEnemyCheck(HitResult);
			//Here HitTagCheck
			HitTagCheck(HitResult);
		}
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}


void UQRGA_Shoot::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetPlayerReference()->bTransADS = false;
	if (ShootForceEndTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootForceEndTimer);
	}

}

APlayerCharacter* UQRGA_Shoot::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_Shoot::CallEndAbility(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//Checks if the Nofity name
	if (NotifyName == FName("FinishedFire"))
	{
		//Ends ability is the animation is done
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	if (NotifyName == FName("ADSFireFinish"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UQRGA_Shoot::ForceEndAbility()
{
	//Ends ability is the animation is done
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_Shoot::ActivateEffects()
{
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	int MaxShotRange = GetPlayerReference()->MaxShotRange;
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX[FMath::RandRange(0,3)], CamCompLocation,FRotator(0,0,0), 0.3, FMath::RandRange(0.9,1.1));
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), CamCompLocation, 1.0f, GetPlayerReference(), 0.0f, AiTags::noiseTag); // Let AI know sound was played
	FVector MuzzleLocation = GetPlayerReference()->MuzzleSphere->GetComponentLocation();
	FRotator MuzzleRotRef = GetPlayerReference()->MuzzleSphere->GetComponentRotation();
	FRotator MuzzleRotation = FRotator(MuzzleRotRef.Pitch, MuzzleRotRef.Yaw + 90, MuzzleRotRef.Roll - 90.0f);
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

FHitResult UQRGA_Shoot::HitScan(float MaxDistance)
{
	//Varaibles for Hitscan check
	FHitResult HitResult;
	FCollisionQueryParams Params;
	//Ignore self
	Params.AddIgnoredActor(GetPlayerReference());
	FHitResult HitScanResult;
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	FVector CamCompLocationWithDeviation = FVector(CamCompLocation.X,CamCompLocation.Y + FMath::RandRange(-5.0f, 5.0f), CamCompLocation.Z + FMath::RandRange(-5.0f, 5.0f));
	GetWorld()->LineTraceSingleByChannel(HitScanResult,CamCompLocation,CamCompLocationWithDeviation + CamCompForwardVector * MaxDistance,ECC_Visibility, Params);
	DrawDebugLine(GetWorld(), CamCompLocation, CamCompLocationWithDeviation + CamCompForwardVector * MaxDistance, FColor::Red,true, 1.0f , 0, 5.0f );
	return HitScanResult;
}

void UQRGA_Shoot::HitEnemyCheck(FHitResult HitInput)
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
			
			//Actiavte hit VFX on hit object/actor
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitVFX, HitInput.Location, HitInput.GetActor()->GetActorRotation());
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
				
			//Uses the out going handle to deal damage
			ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
		}
	}
}

void UQRGA_Shoot::HitTagCheck(FHitResult HitInput)
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

bool UQRGA_Shoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
