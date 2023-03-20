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
#include "EnemyReworkDrone.h"
#include "EnemyReworkHybrid.h"
#include "GameFramework/Actor.h"
#include "BehaviourTree/Utils.h"
#include "EnemySpawner.generated.h"


// ENUM so enemy type can be set in BP
UENUM(BlueprintType)
enum class EEnemyTpeBP : uint8
{
	Melee		UMETA(DisplayName="Melee"),
	Drone		UMETA(DisplayName="Drone"),
	Hybrid		UMETA(DisplayName="Hybrid"),
};

UCLASS()
class THEGREATESCAPE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:
	/// *** Functions *** ///
	AEnemySpawner(); // constructor - Sets default values for this actor's properties
	virtual void Tick(float DeltaTime) override; // Called every frame
	
	void SpawnEnemy(Utilities::EnemyTypes TypeToSpawn, int numOfEnemiesToSpawn); // Spawn an Enemy

protected:
	// *** Functions *** ///
	virtual void BeginPlay() override; // Called when the game starts or when spawned

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// *** Variables *** ///
	
	// Type of enemy that will spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowProtectedAccess = "true"))
	EEnemyTpeBP EnemyTypeToSpawn;

	// Radius Enemies will randomly spawn in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowProtectedAccess = "true"))
	float SpawnTriggerRadius = 1000.0f;

	// Spawner sphere component
	class USphereComponent* SpawnerTrigger; 

private:
	// *** Functions *** ///
	FVector GetRandomLocationInRange(float searchRadius);
	
	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	float SpawnRadius = 1000.0f; // Radius to search

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	int NumOfEnemiesToSpawn = 1; // number of enemies to spawn
	
	TSoftClassPtr<AEnemyRework> MeleeEnemyRef; // weak pointer to melee class
	TSoftClassPtr<AEnemyReworkDrone> DroneEnemyRef; // weak pointer to drone class
	TSoftClassPtr<AEnemyReworkHybrid> HybridEnemyRef; // weak pointer to hybrid class

	static TStaticArray<UClass*, 3> EnemyReferences; // Array of enemy references
	
	FNavLocation Location;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
	//Utilities::EnemyTypes TypeToSpawn = Utilities::EnemyTypes::Melee;
};

