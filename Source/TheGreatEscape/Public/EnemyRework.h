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
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UGameplayAbility> QRGAAttack; // Gameplay ability type (set in bp)

protected:
	// *** Functions *** ///
	virtual void BeginPlay() override; // begin play

private:
	// *** Functions *** ///
	void SetUpStimulus(); // set up stimulus

	// *** Variables *** ///
	class UAIPerceptionStimuliSourceComponent* Stimulus; // stimulus component

};
