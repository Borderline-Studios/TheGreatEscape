// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyMelee.h
// Description : 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AEnemyMelee : public AEnemy
{
	GENERATED_BODY()

	// Override attack function

	AEnemyMelee();

	virtual void Attack() override;
};
