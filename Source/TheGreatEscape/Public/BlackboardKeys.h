// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BlackboardKeys.h
// Description : Lists blackboard keys
// Author      :  Borderline Studios - Toni Natta
// Mail        :  toni.natta@mds.ac.nz

#pragma once
#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace BbKeys
{
	TCHAR const * const targetLocation = TEXT("TargetLocation");
	TCHAR const * const canSeePlayer = TEXT("CanSeePlayer");
	TCHAR const * const patrolPathVector = TEXT("PatrolPathVector");
	TCHAR const * const patrolPathIndex = TEXT("PatrolPathIndex");
	TCHAR const * const playerInMeleeRange = TEXT("PlayerInMeleeRange");
}
