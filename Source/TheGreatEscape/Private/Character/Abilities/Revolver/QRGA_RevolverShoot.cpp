// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Character/Abilities/Revolver/QRGA_RevolverShoot.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Boss.h"
#include "EnemyRework.h"
#include "EnemyReworkController.h"
#include "EnemyReworkDrone.h"
#include "EnemyReworkHybrid.h"
#include "EnemyReworkHybridTank.h"
#include "Enemy_Drone.h"
#include "Enemy_Drone_Bomber.h"
#include "InterpolationHitProxy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "BehaviourTree/Utils.h"
#include "Chaos/ChaosPerfTest.h"
#include "Kismet/GameplayStatics.h"

#include "Character/BASE/GASBASECharacter.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Interactables/WorldInteractTrigger.h"
#include "Kismet/KismetMathLibrary.h"
#include "Objectives/DestroyableTarget.h"
#include "Objectives/ShieldGenerator.h"
#include "Perception/AISenseConfig_Hearing.h"

UQRGA_RevolverShoot::UQRGA_RevolverShoot()
{
	//sets the input key via the Enum
	AbilityInputID = EGASAbilityInputID::Shoot;
}

void UQRGA_RevolverShoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (GetPlayerReference()->bRevolverEquipped)
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

			//Sets ammo to zero to avoid any issues (Probably not nessessary)
			GetPlayerReference()->PlayerAmmo = 0;
			//Sets ammo to full
			GetPlayerReference()->PlayerAmmo = 6;

			//Jumps animontage ot the reload section to player reload animation
			GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("Reload");
			//Checks for an Animnotify then triggers function
			GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_RevolverShoot::CallEndAbility);

			//Plays reload sound at location
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSFX, CamCompLocation,FRotator(0,0,0), 0.3, 1.0);
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
				GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_RevolverShoot::CallEndAbility);
			}
			else
			{
			
				//Jumps the animontage to the fire section
				GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->Montage_JumpToSection("Fire");
				//Added dynamic notify and triggers function if notify is received
				GetPlayerReference()->RevolverMesh1P->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &UQRGA_RevolverShoot::CallEndAbility);
			}
	
		
			FHitResult HitResult = HitScan(GetPlayerReference()->MaxShotRange);
			ActivateTraceParticle(HitResult);
			ActivateEffects(HitResult);
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
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UQRGA_RevolverShoot::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GetPlayerReference()->bTransADS = false;
	if (ShootForceEndTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ShootForceEndTimer);
	}
}

bool UQRGA_RevolverShoot::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

APlayerCharacter* UQRGA_RevolverShoot::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(GetAvatarActorFromActorInfo());
	//Returns Character Ref
	return CharacterRef;
}

void UQRGA_RevolverShoot::CallEndAbility(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
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
	if(NotifyName == FName("FinishedReload"))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}

void UQRGA_RevolverShoot::ForceEndAbility()
{
	//Ends ability is the animation is done
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UQRGA_RevolverShoot::ActivateEffects(FHitResult HitInput)
{
	FVector CamCompLocation = GetPlayerReference()->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CamCompForwardVector = GetPlayerReference()->GetFirstPersonCameraComponent()->GetForwardVector();
	int MaxShotRange = GetPlayerReference()->MaxShotRange;
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSFX[FMath::RandRange(0,3)], CamCompLocation,FRotator(0,0,0), 0.3, FMath::RandRange(0.9,1.1));
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), CamCompLocation, 1.0f, GetPlayerReference(), 0.0f, AiTags::noiseTag); // Let AI know sound was played
	FVector MuzzleLocation = GetPlayerReference()->MuzzleSphere->GetComponentLocation();
	FRotator MuzzleRotRef = GetPlayerReference()->MuzzleSphere->GetComponentRotation();
	FRotator MuzzleRotation = FRotator(MuzzleRotRef.Pitch, MuzzleRotRef.Yaw , MuzzleRotRef.Roll);
	UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleVFX, GetPlayerReference()->MuzzleSphere, FName(GetPlayerReference()->MuzzleSphere->GetName()),
													MuzzleLocation, MuzzleRotation, EAttachLocation::KeepWorldPosition, true, true);
	
	
	float CamControlPitch = GetPlayerReference()->GetController()->GetControlRotation().Pitch;
	float CamControlYaw = GetPlayerReference()->GetController()->GetControlRotation().Yaw;
	float CamControlRoll = GetPlayerReference()->GetController()->GetControlRotation().Roll;
	float AimPunchAmount = 2.1;
				
	//Add crosshair recoil (aim punch)
	GetPlayerReference()->GetController()->SetControlRotation(FRotator(CamControlPitch + AimPunchAmount, CamControlYaw, CamControlRoll));
	//Cam Shake
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(GetPlayerReference()->CamShake, 1.0f);
}

FHitResult UQRGA_RevolverShoot::HitScan(float MaxDistance)
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

void UQRGA_RevolverShoot::HitEnemyCheck(FHitResult HitInput)
{
	if (HitInput.GetActor())
	{
		GetPlayerReference()->CheckBomber(HitInput);
		//Getting the ability system component from the hit actor
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitInput.GetActor());
		//Check if ASC is vaild
		if(ASC)
		{
			GetPlayerReference()->CreateDamageWidget(HitInput, false);
			//Creates damage effect outgoing handle
			FGameplayEffectSpecHandle EffectToApply = MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
			// bool to check if it was found & the value the health equals
			bool bFound;
			float Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetShieldAttribute(), bFound);
			if (Value <= 0 && bFound)
			{
				//Uses the out going handle to deal damage
				ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
			}
			else if (!bFound)
			{
				//Uses the out going handle to deal damage
				ASC->ApplyGameplayEffectSpecToTarget(*EffectToApply.Data.Get(), ASC);
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
				else if (AEnemyReworkHybrid* enemyHybrid = Cast<AEnemyReworkHybrid>(Enemy))
				{
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

			if (ABoss* Boss = Cast<ABoss>(HitInput.GetActor()))
			{
				Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetShieldAttribute(), bFound);
				if (Value <= 0 && bFound)
				{
					Boss->AdjustUIValue(false);
				}
				else
				{
					Boss->AdjustUIValue(true);
				}
				
				Boss->PostHitProcess();
			}
		}
		if (AEnemy_Drone* Drone = Cast<AEnemy_Drone>(HitInput.GetActor()))
		{
			Drone->PostHitProcess();
		}
	}
}

void UQRGA_RevolverShoot::HitTagCheck(FHitResult HitInput)
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
				else if (HitInput.GetActor()->ActorHasTag("Generator"))
				{
					if (AShieldGenerator* ShieldGenerator = Cast<AShieldGenerator>(HitInput.GetActor()))
					{
						ShieldGenerator->PostHitProcess();
					}
				}
				else if (HitInput.GetActor()->ActorHasTag("Target"))
				{
					if (ADestroyableTarget* DestroyableTarget = Cast<ADestroyableTarget>(HitInput.GetActor()))
					{
						DestroyableTarget->DestoryTarget();
					}
				}
			}
		}
	}
}
