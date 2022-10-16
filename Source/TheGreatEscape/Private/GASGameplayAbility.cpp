// Fill out your copyright notice in the Description page of Project Settings.


#include "GASGameplayAbility.h"

UGASGameplayAbility::UGASGameplayAbility()
{
}

EGASAbilityInputID UGASGameplayAbility::GetInputIDAsInt(EGASAbilityInputID ID)
{
	return static_cast<EGASAbilityInputID>(ID); 
}
