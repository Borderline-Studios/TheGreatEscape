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

#include "TrainEngine.h"
#include "Character/Player/PlayerCharacter.h"

/**
 * @brief Sets default values
 */
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
		// Rename the collision profile
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
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
			ProjectileMeshComponent->SetWorldScale3D(FVector((2 * ProjectileRadius) / 100 ));
			ProjectileMeshComponent->SetRelativeLocation(FVector(0, 0, -ProjectileRadius));
		}
		ProjectileMeshComponent->SetupAttachment(RootComponent);
	}

	// Set its initial lifespan to 3 seconds
	InitialLifeSpan = 3.0f;
}

/**
 * @brief Called when the game starts or when spawned
 */
void AHybridEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Add function for on hit
	CollisionComponent->OnComponentHit.AddDynamic(this, &AHybridEnemyProjectile::OnHit);
}

/**
 * @brief Called every frame
 * @param DeltaTime Deltatime
 */
void AHybridEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * @brief Set velocity of the bullet in right direction
 * @param ShootDirection Direction bullet is being shot
 */
void AHybridEnemyProjectile::FireDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *ProjectileMovementComponent->Velocity.ToString());
}

// On collision with another thing
void AHybridEnemyProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("proj uwu other actor = %s"), *OtherActor->GetName());
	
	// check if collided with player
	if (APlayerCharacter* otherActor = Cast<APlayerCharacter>(OtherActor))
	{
		UAbilitySystemComponent* ASC = otherActor->GetAbilitySystemComponent();
		if (ASC)
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(otherActor);
			
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
			ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), ASC);
			UE_LOG(LogTemp, Warning, TEXT("Player dmg proj"));
		}
	}

	// check if it hit the train
	if (ATrainEngine* otherActor = Cast<ATrainEngine>(OtherActor))
	{
		UAbilitySystemComponent* ASC = otherActor->GetAbilitySystemComponent();
		if (ASC)
		{
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(otherActor);
			// Create the Effect Spec from the EffectClass and Effect Context
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
			//FGameplayEffectSpecHandle EffectToApply = GameplayAbility->MakeOutgoingGameplayEffectSpec(GameplayEffectClass);
			ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), ASC);
			UE_LOG(LogTemp, Warning, TEXT("twain dmg proj"));
		}
	}
	
	Destroy();
}
