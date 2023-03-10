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

//AEnemySpawner* spawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(this, AEnemySpawner::StaticClass()));
//spawner->SpawnEnemy(Utilities::EnemyTypes::Melee, 2);

/**
 * @brief Constructor - sets default values
 */
AEnemySpawner::AEnemySpawner()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

/**
 * @brief Called when the game starts or when spawned
 */
void AEnemySpawner::BeginPlay()
{
	  // call super
	  Super::BeginPlay();
	
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
         for (int i = 0; i <= numOfEnemiesToSpawn; i++)
         {
            FVector temp = GetRandomLocationInRange(SearchRadius);
            AEnemyRework* newEnemy = Cast<AEnemyRework>(GetWorld()->SpawnActor(AEnemyRework::StaticClass(), &temp));

          if (newEnemy)
          {
           //newEnemy->SetActorLocation(GetRandomLocationInRange(SearchRadius));
           UE_LOG(LogTemp, Warning, TEXT("Melee enemie spawned :3"));
          }
          else
          {
           UE_LOG(LogTemp, Warning, TEXT("womp womp"));
          }
         }
         break;
      }
     case Utilities::EnemyTypes::Drone:
     {
         for (int i = 0; i <= numOfEnemiesToSpawn; i++)
         {
           AEnemyReworkDrone* newDrone = Cast<AEnemyReworkDrone>(GetWorld()->SpawnActor(AEnemyReworkDrone::StaticClass()));
           newDrone->SetActorLocation(GetRandomLocationInRange(SearchRadius));
           UE_LOG(LogTemp, Warning, TEXT("drone boi spawned goofy ahh"));
         }
         break;
     }
     case Utilities::EnemyTypes::Hybrid:
     {
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


