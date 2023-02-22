// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : NPCPatrolPath.cpp
// Description : Keeps track of the patrol path for the enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "NPCPatrolPath.h"

// Sets default values
ANPCPatrolPath::ANPCPatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

FVector ANPCPatrolPath::GetPatrolPoint(int const index) const
{
	return PatrolPoints[index];
}

int ANPCPatrolPath::Num() const
{
	return  PatrolPoints.Num(); // return number of elements in array
}


