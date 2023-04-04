// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : QRAttributeSet.h
// Description : The header file for the Characters attribute set for GAS
// Author      :  Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz
#pragma once

//includes
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "EngineUtils.h"
#include "QRAttributeSet.generated.h"

//GAS Getters, Setter and Initers - GAS built-in defines
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
	//Constructor
	UQRAttributeSet();


	//UProperty for Health and Max Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRepHealth, Category = "Attribs")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UQRAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRepHealth, Category = "Attribs")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UQRAttributeSet, MaxHealth);

	//UProperty for Health and Max Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRepHealth, Category = "Attribs")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UQRAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRepHealth, Category = "Attribs")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UQRAttributeSet, MaxShield);
	
	/*GAS built-in Functions to set up player stats*/
	void AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,
									 const FGameplayAttributeData& MaxAttribute,
									 float NewMaxValue,
									 const FGameplayAttribute& AffectedAttributeProperty) const;

	UFUNCTION()
	virtual void OnRepHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRepMaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRepShield(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRepMaxShield(const FGameplayAttributeData& OldValue);
	
	/*GAS built-in Functions to set up player stats*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	
	
};
