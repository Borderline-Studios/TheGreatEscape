// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyRework.h"
#include "EnemyReworkDrone.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AEnemyReworkDrone : public AEnemyRework
{
	GENERATED_BODY()

public:
	AEnemyReworkDrone();

	virtual void PossessedBy(AController* NewController) override;
};
