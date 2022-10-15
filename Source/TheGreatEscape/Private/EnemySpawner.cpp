// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnemyTimer -= DeltaTime;

	if (EnemyTimer <= 0.0f)
	{
		EnemyTimer = 5.0f;

		SpawnEnemy();
		
	}

}

void AEnemySpawner::SpawnEnemy()
{
	UWorld* world = GetWorld();

	if (world)
	{
		const int playerIndex = 0;
			
		FVector playerLocation = UGameplayStatics::GetPlayerCharacter(world, playerIndex)->GetActorLocation();

		FVector enemyLocation = playerLocation;

		float randomDistance = FMath::RandRange(10.0f, 500.0f);
			
		enemyLocation.X += randomDistance;
		enemyLocation.Y += randomDistance;

		UE_LOG(LogTemp, Warning, TEXT("Random Location %s"), *enemyLocation.ToString());
		
		world->SpawnActor<AEnemy>(AEnemy::StaticClass(), enemyLocation, FRotator::ZeroRotator);
	}
}


