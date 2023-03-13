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

#include "HybridEnemyProjectile.h"

// Sets default values
AHybridEnemyProjectile::AHybridEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// If it is not the root component, make it
	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile"));
	}
	if(!CollisionComponent)
	{
		// Create sphere component
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the spheres  radius.
		CollisionComponent->InitSphereRadius(ProjectileRadius);
		// Set root component 
		RootComponent = CollisionComponent;
	}

	// Set up projectile movement
	if(!ProjectileMovementComponent)
	{
		// Projectile movement 
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = InitSpeed;
		ProjectileMovementComponent->MaxSpeed = MaxSpeed;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	// Set Mesh
	if(!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
		if(Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

}

// Called when the game starts or when spawned
void AHybridEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHybridEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHybridEnemyProjectile::FireDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

