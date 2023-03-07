// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"

#include "QRAttributeSet.h"
#include "QRAbilitySystemComponent.h"
#include "QRGameplayAbility.h"
#include "Kismet/GameplayStatics.h"

#include "GASBASECharacter.generated.h"

class UQRGameplayAbility;
class UQRAbilitySystemComponent;
class UQRAttributeSet;

UCLASS()
class THEGREATESCAPE_API AGASBASECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGASBASECharacter();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UQRGameplayAbility>> GameplayAbilities;

	UPROPERTY()
	uint8 bAbilitiesInitalized:1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UQRAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UQRAttributeSet> Attributes;

	//Functions
	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo,
				   const struct FGameplayTagContainer& DamageTags,
				   ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamagerCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float Deltavalue, const struct FGameplayTagContainer& EventTags);

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo,
				   const struct FGameplayTagContainer& DamageTags,
				   ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamagerCauser);

	virtual void HandleHealthChanged(float Deltavalue, const struct FGameplayTagContainer& EventTags);
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	void AddStartupGameplayAbilities();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

public:

	float SprintMod = 1.0f;

	/** Handles moving forward/backward */
	virtual  void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual void SetSprintMod(float InVal);

};
