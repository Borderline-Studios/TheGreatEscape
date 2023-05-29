// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : Utils.h
// Description : Lists blackboard keys so retyping is not necessary, also some other Util stuff for other classes
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
	TCHAR const * const bBatteryNearby = TEXT("BatteryNearby");
	TCHAR const * const batteryLocation = TEXT("BatteryLocation");
	TCHAR const * const enemyHealthLow = TEXT("EnemyHealthLow");
	TCHAR const * const randChance = TEXT("RandChance");
	TCHAR const * const stationaryHybrid = TEXT("StationaryHybrid");
	TCHAR const * const playerInMoveRange = TEXT("PlayerInMoveRange");
	TCHAR const * const hasHeardNoise = TEXT("HasHeardNoise");
	TCHAR const * const playerPathExists = TEXT("PlayerPathExists"); 
	TCHAR const * const hybirdHit = TEXT("HybridHit");
}

/**
 * @brief Keeps track of AI tags used for perception
 */
namespace AiTags
{
	TCHAR const * const noiseTag = TEXT("Noise");
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

	// const array of random points for enemies that attack the train
	float const RandTrainXPoints[] = {600.0f,400.0f,200.0f,0.0f,-200.0f,-400.0f,-600.0f};
	int const XPointsLength = 7;

	
}
