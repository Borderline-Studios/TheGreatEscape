// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BlackboardKeys.h
// Description : Lists blackboard keys so retyping is not necesarry
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

/**
 * @brief Holds all blackboard keys
 */
namespace BbKeys
{
	TCHAR const * const targetLocation = TEXT("TargetLocation");
	TCHAR const * const canSeePlayer = TEXT("CanSeePlayer");
	TCHAR const * const patrolPathVector = TEXT("PatrolPathVector");
	TCHAR const * const patrolPathIndex = TEXT("PatrolPathIndex");
	TCHAR const * const playerInMeleeRange = TEXT("PlayerInMeleeRange");
	TCHAR const * const droneInAttackRange = TEXT("DroneInAttackRange");
	TCHAR const * const NPCInPlayerRange = TEXT("NPCInPlayerRange");
	TCHAR const * const NPCCanIncrement = TEXT("NPCCanIncrement");
	TCHAR const * const isTrainClosest = TEXT("IsTrainClosest");
}

/**
 * @brief All enemy types
 */
namespace Utilities
{
	enum class EnemyTypes
	{
		Melee,
		Drone,
		Hybrid,
		NPC
	};

	// const array of
	float const RandTrainXPoints[] = {150.0f,100.0f,50.0f,0.0f,-50.0f,-100.0f,-150.0f};
	int const XPointsLength = 7;
}
