// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "EngineUtils.h"
#include "QRAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
	


/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UQRAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UQRAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRepHealth, Category = "Attribs")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UQRAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRepHealth, Category = "Attribs")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UQRAttributeSet, MaxHealth);

	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
									 const FGameplayAttributeData& MaxAttribute,
									 float NewMaxValue,
									 const FGameplayAttribute& AffectedAttributeProperty) const;

	UFUNCTION()
	virtual void OnRepHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRepMaxHealth(const FGameplayAttributeData& OldValue);

	
	
	
};
