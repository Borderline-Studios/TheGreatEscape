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
	void Idle(float DeltaTime); // Idle for sequence 1 & 2
	void Lasers(float DeltaTime); // Boss' laser
	void DoubleLaser(float DeltaTime); // Resets laser pos & rot
	void ObjDropAttack(float DeltaTime); // Boss' Fist attack
	void ObjDropAttackReset(float DeltaTime); // Resets fists pos & rot
	void Parkour(float DeltaTime); // Parkour stage
	void StartBootUp(float DeltaTime); // Starting boot up 
	void GenShieldUp(float DeltaTime); // Generated shield up
	void PersonalShieldUp(float DeltaTime); // Personal Shield up
	void IdleSeq3(float DeltaTime); // Idle for sequence 3


	// Animation Delegate fucntions
	UFUNCTION()
	void LasersAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION()
	void DoubleLasersAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	
	// *** Variables *** //
	TArray<StateMachine::FStateMachine> StateMachines; // List of all the state machines
	int currentStateMachineIndex = 2; // Index to hold what state machine we are currently on

	TArray<void (ABoss::*)(float)> FunctionPtrs;

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
	float TrackerSpeed = 2.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjDropAttack", meta = (AllowPrivateAccess = "true"))
	float ObjectSpawnHeight = 1500.0f; // Object spawn height

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle", meta = (AllowPrivateAccess = "true"))
	float IdleTimer = 3.0f; // Object spawn height


	// objects
	AActor* Tracker = nullptr; // Tracker obj

	AActor* Laser = nullptr; // Laser left Obj
	
	AActor* DoubleLaserL = nullptr; // Laser left Obj double lasers
	AActor* DoubleLaserR = nullptr; // Laser left Obj double lasers
	
	// Timers
	FTimerHandle TrackerAttackHandle;
	FTimerHandle IdleHandle;
	FTimerHandle IdleSeq3Handle;
	
	// Bools
	// object drop
	bool bTrackerSpawned = false;
	bool bTrackerAttackDone = false;
	bool bObjSpawned = false;

	// laser
	bool bLaserSpawned = false;
	bool bLaserStarted = false;
	bool bLeftLaser = true;

	// double laser
	bool bDoubleLaserSpawned = false;
	bool bDoubleLaserStarted = false;

	// idle
	bool bIdleTimerStarted = false;
	bool bIdleSeq3TimerStarted = false;
};
