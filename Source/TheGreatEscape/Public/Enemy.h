// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : Enemy.h
// Description : 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>

#include "GASAttributeSet.h"
#include "GASAbilitySystemComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class THEGREATESCAPE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:

/* GAS system connection */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS, meta = (AllowPrivateAccess = "true"))
    UGASAbilitySystemComponent* AbilitySystemComponent;

    //Attributes for GAS
    UPROPERTY()
    UGASAttributeSet* Attributes; 
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- Variables ---

	bool bPlayerDetected;
	bool bCanAttackPlayer;
	bool bPlayerIsClosest = false;

	bool bAbilitiesAdded = false;

	class ATheGreatEscapeCharacter* Player;

	class ATheGreatEscapeGameMode* GameMode;
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerDetection;

	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerAttackDetection;

	UPROPERTY(BlueprintReadWrite)
		class USphereComponent* HeadShotSphere;

	class AEnemyAIController* EnemyAIController;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

    virtual void InitializeAttributes();
    virtual void GiveAbilities();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<class UGASGameplayAbility>> DefaultAbilities;

	// --- Functions ---

	UPROPERTY(EditAnywhere)
	float StoppingDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttacking;

	FTimerHandle SeekPlayerTimerHandle;
	FTimerHandle AttackPlayerTimerHandle;
	FTimerHandle WaitTimerHandle;

	UFUNCTION()
		void MoveToPlayer();
	
	UFUNCTION()
		void SeekPlayer();

	UFUNCTION()
		void StopSeekingPlayer();

	UFUNCTION()
		virtual void Attack();

	UFUNCTION()
		void StopAttack();

	UFUNCTION()
		void WaitToRun();

	UFUNCTION()
		void Run();

	UFUNCTION()
		double FindDistanceToPlayer();

	UFUNCTION()
		void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
