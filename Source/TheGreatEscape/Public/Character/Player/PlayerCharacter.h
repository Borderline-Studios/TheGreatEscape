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
#include "NiagaraSystem.h"
#include "../../../../../../../../../../Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/INCLUDE/bit"
#include "Character/BASE/GASBASECharacter.h"
#include "Components/PostProcessComponent.h"
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
	/** First person mesh */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly ,Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category=VFX)
	USphereComponent* MuzzleSphere;
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
	
	bool bFirstDeathCall = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bBatteryPickedUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bRevolverEquipped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bRifleEquipped = false;
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
	int MaxShotRange = 20000;

	//Player Ammo variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int PlayerAmmo = 6;

	int VoiceLineTiggerNum = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentKill = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	float SprintMod = 50;
#pragma endregion 
	FRandomStream Stream;
	
	UFUNCTION(BlueprintImplementableEvent)
	void CallVignette();

	UFUNCTION(BlueprintImplementableEvent)
	void DisableVignette();

	void LoadLevel();
public:
	//Constructor
	APlayerCharacter();

	UFUNCTION(BlueprintCallable)
	void PostHitProcess();

	UFUNCTION(BlueprintCallable)
	void PostDeathProcess();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EnemyKilled();
	
	//Functions
	virtual void Tick(float DeltaSeconds) override;
	UCameraComponent* GetFirstPersonCameraComponent();
	
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerReference();
	
	UFUNCTION(BlueprintCallable)
	void SetResources(int ValueToChange, int NewValue);
};
