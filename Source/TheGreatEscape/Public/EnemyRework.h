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
	void CalcRandomAttackPos(); // Function for train attacking enemies to use so they do not go after same point
	void PostDeathProcess();

	float CheckHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* DeathEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* DeathSFX;

	// *** Variables *** ///
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UGameplayAbility> QRGAAttack; // Gameplay ability type (set in bp)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TrainTargetPointOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bAttacking = false;



protected:
	// *** Functions *** ///
	virtual void BeginPlay() override; // begin play

	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowProtectedAccess = "true"))
	float yOffsetFromTrain = 225.0f; // Y axis offset from train

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float ElevationHeight = 550.0f; // height above train the enemy will float

private:
	// *** Functions *** ///
	void SetUpStimulus(); // set up stimulus

	// *** Variables *** ///
	class UAIPerceptionStimuliSourceComponent* Stimulus; // stimulus component

	bool FirstDeath = true;

};
