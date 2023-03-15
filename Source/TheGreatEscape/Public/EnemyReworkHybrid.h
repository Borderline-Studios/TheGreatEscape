// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkHybrid.h
// Description : Hybrid enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "EnemyRework.h"
#include "EnemyReworkHybrid.generated.h"

/**
 * Hybrid enemy
 */
UCLASS()
class THEGREATESCAPE_API AEnemyReworkHybrid : public AEnemyRework
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	AEnemyReworkHybrid(); // constructor
	virtual void PossessedBy(AController* NewController) override; // possessed by
	virtual void Tick(float DeltaTime) override; // tick, called every frame

	// *** Variables *** ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Mesh")
	UStaticMeshComponent* LeftTurretRef; // ref to turret

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Mesh")
	UStaticMeshComponent* RightTurretRef; // ref to turret 
};
