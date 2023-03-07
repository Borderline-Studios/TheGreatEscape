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
};
