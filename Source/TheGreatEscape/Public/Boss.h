// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : Boss.h
// Description : Main code for the boss
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Character/BASE/GASBASECharacter.h"
#include "BossStateMachine.h"
#include "Boss.generated.h"


/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API ABoss : public AGASBASECharacter
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	ABoss(); // constructor
	virtual void Tick(float DeltaTime) override; // tick, called every frame

private:
	// *** Functions *** ///
	void StateMachineSetUps();

	// Delegate functions
	void Laser(); // Boss' laser
	void LaserReset(); // Resets laser pos & rot
	void Fist(); // Boss' Fist attack
	void FistReset(); // Resets fists pos & rot
	void Parkour(); // Parkour stage

	// *** Variables *** //
	TArray<StateMachine::FStateMachine> StateMachines; // List of all the state machines

	int currentStateMachineIndex = 0; // Index to hold what state machine we are currently on

	bool temp = false;
};
