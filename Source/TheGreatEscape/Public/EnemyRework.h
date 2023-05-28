// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyRework.h
// Description : Parent Enemy class 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz
#pragma once

#include <Character/BASE/GASBASECharacter.h>
#include "CoreMinimal.h"
#include "NiagaraEmitter.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyRework.generated.h"

UCLASS()
class THEGREATESCAPE_API AEnemyRework : public AGASBASECharacter
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	AEnemyRework(); // constructor
	virtual void PossessedBy(AController* NewController) override; // Possessed by
	virtual void Tick(float DeltaTime) override; // tick, called every frame
    void Attack(); // attack function
	// Function for train attacking enemies to use so they do not go after same point
	void CalcRandomAttackPos();
	UFUNCTION(BlueprintCallable)
	virtual void PostDeathProcess();
	UFUNCTION(BlueprintCallable)
	virtual void PostHitProcess();

	float CheckHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* DeathEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* DeathSFX;

	//Sound effect declearation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	TArray<USoundBase*> EnemyHitSFX;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SFX")
	USoundBase* DronePewPew;
	
	// *** Variables *** ///
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UGameplayAbility> QRGAAttack; // Gameplay ability type (set in bp)


	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TrainTargetPointOffset;

	UFUNCTION(BlueprintImplementableEvent)
	void ShieldBrakeChangeMaterial();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bAttacking = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	bool bCloseUpEnemy = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float ElevationHeight = 600.0f; // height above train the enemy will float

	// Used Jacob's method to implement SFX
	int SFXTiggerNum = FMath::RandRange(1,4);

	TSoftClassPtr<AActor> DeathEffects; // weak pointer to the death effect

	UClass* DeathEffectReference; // Array of enemy references


protected:
	// *** Functions *** ///
	virtual void BeginPlay() override; // begin play

	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowProtectedAccess = "true"))
	float yOffsetFromTrain = 700.0f; // Y axis offset from train

	

private:
	// *** Functions *** ///
	void SetUpStimulus();// set up stimulus

	// *** Variables *** ///
	class UAIPerceptionStimuliSourceComponent* Stimulus; // stimulus component

	bool FirstDeath = true;

};
