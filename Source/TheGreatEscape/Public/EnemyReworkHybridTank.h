// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkHybridTank.h
// Description : Hybrid Tank enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "EnemyReworkHybrid.h"
#include "EnemyReworkHybridTank.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AEnemyReworkHybridTank : public AEnemyReworkHybrid
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	AEnemyReworkHybridTank(); // Constructor
	virtual void PossessedBy(AController* NewController) override; // possessed by
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override; // tick, called every frame
	virtual void PostDeathProcess() override;
	virtual void PostHitProcess() override;

private:
	// *** Functions *** ///
	UFUNCTION()
	void ShootGun(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	void SetCanAttack();
	
	// *** Variables *** ///
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSoftClassPtr<AHybridEnemyProjectile> ProjectileClass; // weak pointer to class type we want

	FActorSpawnParameters SpawnParams;
	
	UClass* LoadedBpProjectile; // Class to spawn

	FTimerHandle AttackDelayHandle; // Timer handle to handle the attack delay
	bool bCanAttack = true; // bool to check if drone can attack

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackDelay = 5.0f; // Delay between attacks

	ACharacter* PlayerRef;
	
};