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

#include "CoreMinimal.h"
#include "NPCPatrolPath.h"
#include "QRAbilitySystemComponent.h"
#include "GameFramework/Character.h"

#include "QRAttributeSet.h"
#include "QRAbilitySystemComponent.h"
#include "QRGameplayAbility.h"
#include "Character/QRCharacter.h"
#include "Kismet/GameplayStatics.h"

#include "EnemyRework.generated.h"

class UQRGameplayAbility;
class UQRAbilitySystemComponent;
class UQRAttributeSet;

UCLASS()
class THEGREATESCAPE_API AEnemyRework : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyRework();

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
				   AQRCharacter* InstigatorCharacter, AActor* DamagerCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float Deltavalue, const struct FGameplayTagContainer& EventTags);

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo,
				   const struct FGameplayTagContainer& DamageTags,
				   AQRCharacter* InstigatorCharacter, AActor* DamagerCauser);

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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ANPCPatrolPath* GetPatrolPath();

	void Attack(UAbilitySystemComponent* TargetActorASC);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

private:
	class UAIPerceptionStimuliSourceComponent* Stimulus;
	void SetUpStimulus();

	// NPC STUFF
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	ANPCPatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffect", meta = (AllowPrivateAccess = "true"))
	UGameplayEffect* DamageEffect;

	
};
