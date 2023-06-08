// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : PlayerCharacter.h
// Description : The Header file for the player character using the GASBASECharacter class
// Author      : Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once

//Includes
#include "CoreMinimal.h"
#include "Character/BASE/GASBASECharacter.h"
#include "PlayerCharacter.generated.h"

//Forward Declaration
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class USphereComponent;
class UAnimMontage;
class USoundBase;
/**
 * 
 */
UCLASS(Config=Game)
class THEGREATESCAPE_API APlayerCharacter : public AGASBASECharacter
{
	GENERATED_BODY()
	
public:
#pragma region Components
	/* Revolver Mesh*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly ,Category=Mesh)
	USkeletalMeshComponent* RevolverMesh1P;

	/* Rifle Mesh*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly ,Category=Mesh)
	USkeletalMeshComponent* RifleMesh1P;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category=VFX)
	USphereComponent* MuzzleSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category=VFX)
	USphereComponent* RifleMuzzleSphere;
#pragma endregion

#pragma region Animation
	//Animation Montage variable at acces the animation montage in code
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ShootMontage;
#pragma endregion

#pragma region Booleans
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bSprinting = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bTransADS = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bADS = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bBossLevel = false; 
	
	bool bFirstDeathCall = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bRevolverEquipped = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bRifleEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bRiflePickedUp = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bBatteryPickUp = false;

#pragma endregion 

#pragma region SFX
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CamShake;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX",  meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*> QuipSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX",  meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*> HitSFX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	USoundBase* ButtonSFX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	USoundBase* JumpSFX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	USoundBase* LandSFX;

	//Sound effect declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX", meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*> LeverSFX;
#pragma endregion

#pragma region Integers-And-Floats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int NumScrap = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int Resource2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int RandomInt = 0; // can remove no issue
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int MaxShotRange = 5000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int MaxRifleShotRange = 10000;

	//Player Ammo variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int PlayerAmmo = 6;

	//Player Ammo variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int RifleAmmo = 30;
	//Player Ammo variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int CurrentRifleAmmo = RifleAmmo;
	int VoiceLineTiggerNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentKill = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	float SprintMod = 50;

private:
	//Player Battery Counter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int PlayerBatteryCount = 0;
	
public:
#pragma endregion

    //QRGameplayAbility* CurrentAbility = nullptr;
	
	FRandomStream Stream;

	void IncrementBatteryCount();
	void DecrementBatteryCount();
	int GetPlayerBatteryCount() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool PlayerHasBattery();

	UFUNCTION(BlueprintImplementableEvent)
	void HitVignette();

	UFUNCTION(BlueprintImplementableEvent)
	void CheckBomber(FHitResult HitInput);

	UFUNCTION(BlueprintImplementableEvent)
	void PickUpRobopal();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CallVignette();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DisableVignette();
	
	UFUNCTION(BlueprintImplementableEvent)
	void LerpFOV(float From, float To, bool Reverse);

	//Lerp Location for Crouch
	UFUNCTION(BlueprintImplementableEvent)
	void LerpLocation(float From, float To, bool Reverse);

	UPROPERTY(BlueprintReadWrite)
	FVector RespawnLocation;

	//Declearation of effect to apply when enemy is hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	void LoadLevel();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bResetCurrrentLevel = true;
public:
	//Constructor
	APlayerCharacter();

	UFUNCTION(BlueprintCallable)
	void PostHitProcess();

	UFUNCTION(BlueprintCallable)
	void PostDeathProcess();

	UFUNCTION(BlueprintImplementableEvent)
	void RocketTargetWidget(AActor* InActor);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveRocketTargetWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ResetLevel();
	
	//Functions
	virtual void Tick(float DeltaSeconds) override;
	UCameraComponent* GetFirstPersonCameraComponent();
	
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerReference();

	UFUNCTION(BlueprintImplementableEvent)
	void ReApplyPassives();

	UFUNCTION(BlueprintImplementableEvent)
	void ApplyUI();

	UPROPERTY(BlueprintReadWrite);
	bool AutoApplyUI = true;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CreateDamageWidget(FHitResult HitResult, float Damage, bool ShieldDamage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ActivateRocket();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DeactivateRocket();

	void Jump() override;

	void StopJumping() override;

	void Landed(const FHitResult& Hit) override;
};
