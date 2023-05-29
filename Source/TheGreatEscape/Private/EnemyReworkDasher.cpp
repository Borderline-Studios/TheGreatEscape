// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyRework.cpp
// Description : Dashing Vairent
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "EnemyReworkDasher.h"

#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyReworkDasher::AEnemyReworkDasher()
{
	// constructor
}

void AEnemyReworkDasher::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AEnemyReworkDasher::BeginPlay()
{
	Super::BeginPlay();
	
	// Set is Dasher varient
	if (const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(GetController()))
	{
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::isMelee, false);
	}
}

void AEnemyReworkDasher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyReworkDasher::PostDeathProcess()
{
	Super::PostDeathProcess();
}

void AEnemyReworkDasher::PostHitProcess()
{
	Super::PostHitProcess();
}
