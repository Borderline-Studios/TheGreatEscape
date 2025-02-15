// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworKDrone.h
// Description : Drone enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#pragma once

#include "CoreMinimal.h"
#include "EnemyRework.h"
#include "EnemyReworkDrone.generated.h"

/**
 * Enemy Drone
 */
UCLASS()
class THEGREATESCAPE_API AEnemyReworkDrone : public AEnemyRework
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	AEnemyReworkDrone(); // constructor
	virtual void PossessedBy(AController* NewController) override; // possessed by
	virtual void Tick(float DeltaTime) override; // tick, called every frame

	virtual void PostDeathProcess() override;
	virtual void PostHitProcess() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Mesh")
	UStaticMeshComponent* TurretBaseRef; // ref to turret base

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float Time = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float TimeIncrement = 0.005f;
	
	float LastDist;
};
