// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : GASBASECharacter.h
// Description : HeaderFile for the GASBASECharacter - Child of UEs Character class just with added Gameplay Ability System Functionality
// Author      :  Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once

//TODO Use private and protected more often

//Base UE includes
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

//GamplayAbilitySystem Includes
#include "AbilitySystemInterface.h"
#include "QRAttributeSet.h"
#include "QRAbilitySystemComponent.h"
#include "QRGameplayAbility.h"
#include "Kismet/GameplayStatics.h"

#include "GASBASECharacter.generated.h"


//Forawrd Declarations of Specific GAS classes made by me
class UQRGameplayAbility;
class UQRAbilitySystemComponent;
class UQRAttributeSet;

UCLASS()
class THEGREATESCAPE_API AGASBASECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//Constructor
	AGASBASECharacter();

	//Default Passive and Activateable abilities/effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UQRGameplayAbility>> GameplayAbilities;

	//GAS specific variables
	UPROPERTY()
	uint8 bAbilitiesInitalized:1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UQRAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UQRAttributeSet> Attributes;

	//GAS Functions
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo,
				   const struct FGameplayTagContainer& DamageTags,
				   AGASBASECharacter* InstigatorCharacter, AActor* DamagerCauser);
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float Deltavalue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo,
				   const struct FGameplayTagContainer& DamageTags,
				   AGASBASECharacter* InstigatorCharacter, AActor* DamagerCauser);
	virtual void HandleHealthChanged(float Deltavalue, const struct FGameplayTagContainer& EventTags);
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	void AddStartupGameplayAbilities();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	//UE Movement Functions
	virtual  void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;
};
