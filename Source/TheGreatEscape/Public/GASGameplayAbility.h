
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TheGreatEscape/TheGreatEscape.h"
#include "GASGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UGASGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UGASGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;

	UFUNCTION(BlueprintCallable)
	void SetInputID(EGASAbilityInputID ID);
	
	
};