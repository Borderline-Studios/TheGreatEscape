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
		//float TimerInRate = 60 / FireRate;
		//GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &UQRGA_RifleShoot::FireLoop, TimerInRate, true, -1);
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
			//Uses the out going handle to deal damage
			ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
		}
		else if (AObjectiveShield* ObjectiveShield = Cast<AObjectiveShield>(HitInput.GetActor()))
		{
			ObjectiveShield->PostHitProcess();
		}
	}
}
