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
	/** First person mesh */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly ,Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	//Animation Montage variable at acces the animation montage in code
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ADSMontage;
	//Player Ammo variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
	int PlayerAmmo = 6;
	//Print ability player check
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int NumScrap = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int Resource2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	bool bIsADS = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CamShake;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*> QuipSFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerStats,  meta = (AllowPrivateAccess = "true"))
	int MaxShotRange = 20000;

	FRandomStream Stream;
	
public:
	//Constructor
	APlayerCharacter();

	UFUNCTION(BlueprintImplementableEvent)
	void PostHitProcess();
	
	//Functions
	virtual void Tick(float DeltaSeconds) override;
	void StartDeath();
	UCameraComponent* GetFirstPersonCameraComponent();
	
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerReference();
	
	UFUNCTION(BlueprintCallable)
	void SetResources(int ValueToChange, int NewValue);
};
