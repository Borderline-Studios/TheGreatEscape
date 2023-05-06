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
#include "Character/Player/PlayerCharacter.h"
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
	virtual void BeginPlay() override;
	

private:
	// *** Functions *** ///
	void StateMachineSetUps();

	// Delegate functions
	void Tempfunction(float DeltaTime); // Boss' laser
	void Laser(float DeltaTime); // Boss' laser
	void DoubleLaser(float DeltaTime); // Resets laser pos & rot
	void ObjDropAttack(float DeltaTime); // Boss' Fist attack
	void ObjDropAttackReset(float DeltaTime); // Resets fists pos & rot
	void Parkour(float DeltaTime); // Parkour stage

	// *** Variables *** //
	TArray<StateMachine::FStateMachine> StateMachines; // List of all the state machines
	int currentStateMachineIndex = 1; // Index to hold what state machine we are currently on

	// Player ref
	APlayerCharacter* PlayerRef;

	// Objects to be spawned
	TSoftClassPtr<AActor> LaserRef; // weak pointer to laser class
	UClass* LaserClassRef; // Reference to laser

	TSoftClassPtr<AActor> TrackerRef; // weak pointer to tracker class
	UClass* TrackerClassRef; // Reference to tracker

	TSoftClassPtr<AActor> ObjDroppedRef; // weak pointer to object dropped class
	UClass* ObjDroppedClassRef; // Reference to Object dropped
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjDropAttack", meta = (AllowPrivateAccess = "true"))
	float MaxTrackerTime = 4.0f; // max tracker time

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjDropAttack", meta = (AllowPrivateAccess = "true"))
	float ObjectSpawnHeight = 300.0f; // max tracker time


	AActor* Tracker = nullptr; // Tracker obj
	
	// Timers
	FTimerHandle TrackerAttackHandle;
	
	// Bools
	bool bTrackerSpawned = false;
	bool bTrackerAttackDone = false;
	bool bObjSpawned = false;
};
