// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TheGreatEscape/TheGreatEscape.h"
#include "QRGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UQRGameplayAbility();

	//Abilities will activate when input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	EGASAbilityInputID AbilityInputID {EGASAbilityInputID::None};
	
};
