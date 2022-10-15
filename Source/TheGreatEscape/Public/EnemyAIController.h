// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyAIController.h
// Description : 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
	void BeginPlay() override;

private:
	class UNavigationSystemV1* NavArea;

	FVector RandomLocation;

public:

	UFUNCTION()
		void RandomPatrol();
	
};
