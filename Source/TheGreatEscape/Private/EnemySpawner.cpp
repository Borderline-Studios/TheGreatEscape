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

#include "EnemySpawner.h"
#include "EnemyReworkDrone.h"
#include "NavigationSystem.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/SphereComponent.h"

// Set up static array
TStaticArray<UClass*, 3> AEnemySpawner::EnemyReferences;

/**
 * @brief Constructor - sets default values
 */
AEnemySpawner::AEnemySpawner()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

 // set static array with tree references
 for (int i = 0; i < 3; i++)
 {
   if (!EnemyReferences[i])
   {
      UClass* uClassTemp = nullptr;

      if (i == 0) // MELEE
      { 
      	 MeleeEnemyRef = TSoftClassPtr<AEnemyRework>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Rework/BP_EnemyRework.BP_EnemyRework_C'")));
         uClassTemp = MeleeEnemyRef.LoadSynchronous();
      }
      else if (i == 1) // DRONE
      {
      	 DroneEnemyRef = TSoftClassPtr<AEnemyReworkDrone>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Rework/BP_EnemyReworkDrone.BP_EnemyReworkDrone_C'")));
      	 uClassTemp = DroneEnemyRef.LoadSynchronous();
      }
      else if (i == 2) // HYBRID
      {
         HybridEnemyRef = TSoftClassPtr<AEnemyReworkHybrid>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Rework/BP_EnemyRewordHybrid.BP_EnemyRewordHybrid_C")));
         uClassTemp = HybridEnemyRef.LoadSynchronous();
      }

      EnemyReferences[i] = uClassTemp;
			
  }
 }

	// Set up the sphere collision
	// If it is not the root component, make it
	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile"));
	}
	if(!SpawnerTrigger)
	{
		// Create sphere component
		SpawnerTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Rename the collision profile
		SpawnerTrigger->BodyInstance.SetCollisionProfileName(TEXT("SpawnerTrigger"));
		// Set the spheres  radius.
		SpawnerTrigger->InitSphereRadius(SpawnTriggerRadius);
		// Set root component 
		RootComponent = SpawnerTrigger;
	}
}

/**
 * @brief Called when the game starts or when spawned
 */
void AEnemySpawner::BeginPlay()
{
	  // call super
	  Super::BeginPlay();
	
	// Set Delegate function
	SpawnerTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::OnActorOverlap);
	
}

void AEnemySpawner::OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("spanwer collision = %s"), *OtherActor->GetName());
	
	// check if collided with player
	if (APlayerCharacter* otherActor = Cast<APlayerCharacter>(OtherActor))
	{
		

		switch (EnemyTypeToSpawn)
		{
			case EEnemyTpeBP::Melee:
				{
					SpawnEnemy(Utilities::EnemyTypes::Melee, NumOfEnemiesToSpawn);
					break;
				}
			case EEnemyTpeBP::Drone:
				{
					SpawnEnemy(Utilities::EnemyTypes::Drone, NumOfEnemiesToSpawn);
					break;
				}
			case EEnemyTpeBP::Hybrid:
				{
					SpawnEnemy(Utilities::EnemyTypes::Hybrid, NumOfEnemiesToSpawn);
					break;
				}
			default:
				{
					UE_LOG(LogTemp, Warning, TEXT("Spawner Enum Default value"));
					break;
				}
		}
	}
}

/**
 * @brief Called every frame
 * @param DeltaTime deltatime
 */
void AEnemySpawner::Tick(float DeltaTime)
{
   // call super
  	Super::Tick(DeltaTime);
}

/**
 * @brief Spawns enemies
 * @param TypeToSpawn Type of enemy to spawn
 * @param numOfEnemiesToSpawn Number of enemies to spawn
 */
void AEnemySpawner::SpawnEnemy(Utilities::EnemyTypes TypeToSpawn, int numOfEnemiesToSpawn)
{
   // Check which enemy and spawn them
   switch (TypeToSpawn)
   {
     case Utilities::EnemyTypes::Melee:
      {
         for (int i = 0; i < numOfEnemiesToSpawn; i++)
         {
            FVector temp = GetRandomLocationInRange(SpawnRadius);
            AEnemyRework* newEnemy = Cast<AEnemyRework>(GetWorld()->SpawnActor(EnemyReferences[0], &temp));

         }
         break;
      }
     case Utilities::EnemyTypes::Drone:
     {
         for (int i = 0; i < numOfEnemiesToSpawn; i++)
         {
         	FVector temp = GetRandomLocationInRange(SpawnRadius);
         	AEnemyReworkDrone* newDrone = Cast<AEnemyReworkDrone>(GetWorld()->SpawnActor(EnemyReferences[1], &temp));
         	
         }
         break;
     }
     case Utilities::EnemyTypes::Hybrid:
     {
     		for (int i = 0; i < numOfEnemiesToSpawn; i++)
     		{
     			FVector temp = GetRandomLocationInRange(SpawnRadius);
     			AEnemyReworkHybrid* newHybrid = Cast<AEnemyReworkHybrid>(GetWorld()->SpawnActor(EnemyReferences[2], &temp));
     			
     		}
         break;
     }
     default:
     {
         break;
     }
   }
}

/**
 * @brief Get a random location around the spawner to spawn the enemy at
 * @param searchRadius Radius to search for random location
 */
FVector AEnemySpawner::GetRandomLocationInRange(float searchRadius)
{
   // Get nav system and generate random location near the player
   UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
   if (NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), searchRadius, Location, nullptr))
   {
		    return Location.Location;
   }
	 return FVector::Zero();
}


