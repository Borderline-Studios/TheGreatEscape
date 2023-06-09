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
#include "NiagaraSystem.h"
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
	//~ABoss(); // constructor
	virtual void Tick(float DeltaTime) override; // tick, called every frame
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void PostHitProcess();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ActivatePlatforms();
	UFUNCTION(BlueprintImplementableEvent)
	void DeactivatePlatforms();

	UFUNCTION(BlueprintImplementableEvent)
	void EndGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AdjustUIValue(bool Shield);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Seq2ShieldSetup();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* Phase2StartSFX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USoundBase* Phase3StartSFX;

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

	void NewSequenceEffect(int NewSequenceNum);

	bool CheckSwitchSequence();



	// Animation Delegate fucntions
	UFUNCTION()
	void LasersAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION()
	void DoubleLasersAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION()
	void StartUpAnimsNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	
	// *** Variables *** //
	TArray<StateMachine::FStateMachine> StateMachines; // List of all the state machines
	int currentStateMachineIndex = 0; // Index to hold what state machine we are currently on

	TArray<void (ABoss::*)(float)> FunctionPtrs;

	// Player ref
	APlayerCharacter* PlayerRef;

	// Objects to be spawned
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> LaserSubclassRef;
	TSoftClassPtr<AActor> LaserRef; // weak pointer to laser class
	UClass* LaserClassRef; // Reference to laser

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> TrackerSubclassRef;
	TSoftClassPtr<AActor> TrackerRef; // weak pointer to tracker class
	UClass* TrackerClassRef; // Reference to tracker

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ObjDroppedSubclassRef;
	TSoftClassPtr<AActor> ObjDroppedRef; // weak pointer to object dropped class
	UClass* ObjDroppedClassRef; // Reference to Object dropped

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> GenShieldSubclassRef;
	TSoftClassPtr<AActor> GenShieldRef; // Gen shield
	UClass* GenShieldClassRef; // Reference to generated shield

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BlockingBoxSubclassRef;
	TSoftClassPtr<AActor> BlockingBoxRef; // Blocking box
	UClass* BlockingBoxClassRef; // Reference to blocking box
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> DeathEffectSubclassRef;
	TSoftClassPtr<AActor> DeathEffectRef; // Death Effect ref
	UClass* DeathEffectClassRef; // Reference to Death Effect

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* DeathEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjDropAttack", meta = (AllowPrivateAccess = "true"))
	float MaxTrackerTime = 4.0f; // max tracker time

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjDropAttack", meta = (AllowPrivateAccess = "true"))
	float TrackerSpeed = 2.0f; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjDropAttack", meta = (AllowPrivateAccess = "true"))
	float ObjectSpawnHeight = 1500.0f; // Object spawn height

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjDropAttack", meta = (AllowPrivateAccess = "true"))
	float PlayerFeetZ = 9174.7f; // Z height of floor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle", meta = (AllowPrivateAccess = "true"))
	float IdleTimer = 3.0f;// Object spawn height
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "End Game", meta = (AllowPrivateAccess = "true"))
	float EndGameTimer = 10.0f;// End Game Timer

	// objects
	AActor* Tracker = nullptr; // Tracker obj

	AActor* Laser = nullptr; // Laser left Obj
	
	AActor* DoubleLaserL = nullptr; // Laser left Obj double lasers
	AActor* DoubleLaserR = nullptr; // Laser left Obj double lasers

	TArray<AActor*> FoundGens; // found shield gens
	TArray<AActor*> FoundBlockers; // found blocking ____

	FName LeftSocket;
	FName RightSocket;


	FVector LeftSocketLoc;
	FVector RightSocketLoc;

	FRotator LeftSocketRot;
	FRotator RightSocketRot;
	
	// Timers
	FTimerHandle TrackerAttackHandle;
	FTimerHandle IdleHandle;
	FTimerHandle IdleSeq3Handle;
	FTimerHandle ObjDropResetHandle;
	FTimerHandle EndGameHandle;
	
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
	bool bObjDropResetStarted = false;

	bool bBootUpStarted = false;
	bool bGenShieldUp = false;
	bool bPersonalShieldUp = false;

	bool bGenShieldAnim = false;

	// switching states
	bool bRequestSwitch = false;
	bool bRequestEndGame = false;

	bool bParkourUp = false;
};
