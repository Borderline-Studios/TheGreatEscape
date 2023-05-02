// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>

#include "GameplayEffect.h"
#include "Kismet/GameplayStatics.h"
#include "TheGreatEscapeCharacter.generated.h"



class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

class UQRGameplayAbility;
class UQRAbilitySystemComponent;
class UQRAttributeSet;



// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class ATheGreatEscapeCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
public:
	ATheGreatEscapeCharacter();

	
	//Components
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns RevolverMesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent;}


	//Properties
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	bool FootStep = false;
		
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHitEnemy = false;
	
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
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UQRGameplayAbility>> DefaultAbilities;


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
	

	virtual void InitializeAttributes();
	virtual void GiveAbilities();



protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	/** Fires a projectile. */
	void OnPrimaryAction();
	void OnMelee();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

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

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

	bool bAbilitiesAdded = false;


};

