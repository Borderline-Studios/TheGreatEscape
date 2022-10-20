// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyRanged.h
// Description : 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyRanged.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AEnemyRanged : public AEnemy
{
	GENERATED_BODY()

	virtual void Attack() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ATheGreatEscapeProjectile> Projectile;
};
