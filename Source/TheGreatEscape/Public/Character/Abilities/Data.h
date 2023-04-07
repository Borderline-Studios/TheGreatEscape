// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : Data.h
// Description : Header file containing any relevent data for the Gameplay Ability System
// Author      : Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

#pragma once


// GAS emun for player inputs
UENUM(BlueprintType)
enum class EGASAbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Shoot,
	AimDownSights,
	Reload,
	Melee,
	QAbility,
	EAbility,
	Interact,
	DroneShoot,
	MeleeEnemyAttack,
	Sprint,
	Gather,
	EquipRevolver,
	EquipRifle,
};