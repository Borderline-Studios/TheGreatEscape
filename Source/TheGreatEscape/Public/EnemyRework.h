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

#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviourTree/BlackboardKeys.h"
#include "EnemyReworkController.h"

#include "EnemyRework.generated.h"

UCLASS()
class THEGREATESCAPE_API AEnemyRework : public AGASBASECharacter
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	AEnemyRework();
	
	virtual void PossessedBy(AController* NewController) override;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Attack();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> QRGAAttack;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FGameplayAbilitySpecHandle* QRQAAttackSpecHandle;
		

private:
	class UAIPerceptionStimuliSourceComponent* Stimulus;
	void SetUpStimulus();
};
