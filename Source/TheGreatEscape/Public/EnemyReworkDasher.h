// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkDasher.h
// Description : Dashing varient of rework
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "EnemyRework.h"
#include "EnemyReworkDasher.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AEnemyReworkDasher : public AEnemyRework
{
	GENERATED_BODY()

public:
	AEnemyReworkDasher(); // Constructor
	virtual void PossessedBy(AController* NewController) override; // possessed by
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override; // tick, called every frame
	virtual void PostDeathProcess() override;
	virtual void PostHitProcess() override;

private:
	
	
};
