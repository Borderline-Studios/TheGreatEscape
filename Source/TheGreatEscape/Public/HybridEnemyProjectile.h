// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : HybridEnemyProjectile.h
// Description : Projectile for hybrid enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HybridEnemyProjectile.generated.h"

UCLASS()
class THEGREATESCAPE_API AHybridEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// *** Functions *** //
	AHybridEnemyProjectile(); // Sets default values for this actor's properties
	virtual void Tick(float DeltaTime) override; // Called every frame
	
	void FireDirection(const FVector& ShootDirection); //  initializes the projectiles velocity in the direction it was shot

	// *** Variables *** //
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent; // Sphere collision component
	
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent; // Projectile movement component.
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* ProjectileMeshComponent; // Projectile mesh
	
protected:
	// *** Functions *** //
	virtual void BeginPlay() override; // Called when the game starts or when spawned

	// *** Variables *** //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float ProjectileRadius = 5.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float InitSpeed = 2000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float MaxSpeed = 2000.0f; 
};
