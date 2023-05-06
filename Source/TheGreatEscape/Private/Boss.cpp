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

#include "Boss.h"

ABoss::ABoss()
{
	// Set Up the state machines & their states
	StateMachineSetUps();

	// Load the laser
	LaserRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Boss/Laser.Laser_C'")));
	LaserClassRef = LaserRef.LoadSynchronous();

	// Load the tracker
	TrackerRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Boss/Tracker.Tracker_C'")));
	TrackerClassRef = TrackerRef.LoadSynchronous();

	// Load the object to be spawned
	ObjDroppedRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Boss/ObjectDropped.ObjectDropped_C'")));
	ObjDroppedClassRef = ObjDroppedRef.LoadSynchronous();
	
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Reference failed to load... Boss.cpp"));
	}

}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// INDEX IS UPDATED WHEN DAMAGED
	(this->*(FunctionPtrs[StateMachines[currentStateMachineIndex].CurrentState->index]))(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("index %d"), StateMachines[currentStateMachineIndex].CurrentState->index);

}

void ABoss::StateMachineSetUps()
{
#pragma region CreateVariables	
	// STATES
	StateMachine::FState* StartingBootUpState = new StateMachine::FState; // Sequence 1 - Starting animation 
	StateMachine::FState* IdleSeq1State = new StateMachine::FState; // Sequence 1 - Idle
	StateMachine::FState* LaserSeq1State = new StateMachine::FState; // Sequence 1 - Laser 

	StateMachine::FState* ShieldUpSeq2 = new StateMachine::FState; // Sequence 2 - Generated shield up
	StateMachine::FState* ParkourModeState = new StateMachine::FState; // Sequence 2 - Parkour
	StateMachine::FState* IdleSeq2State = new StateMachine::FState; // Sequence 2 - Idle
	StateMachine::FState* ObjDropSeq2State = new StateMachine::FState; // Sequence 2 - Fist Attack 
	StateMachine::FState* ObjDropResetSeq2State = new StateMachine::FState; // Sequence 2 - Fist Attack Reset

	StateMachine::FState* PersonalShieldSeq3 = new StateMachine::FState; // Sequence 3 - Personal shield up
	StateMachine::FState* IdleSeq3State = new StateMachine::FState; // Sequence 3 - Idle
	StateMachine::FState* DoubleLaserSeq3State = new StateMachine::FState; // Sequence 3 - Double Laser
	StateMachine::FState* ObjDropSeq3State = new StateMachine::FState; // Sequence 3 - ObjDrop Attack 
	StateMachine::FState* ObjDropResetSeq3State = new StateMachine::FState; // Sequence 3 - Fist Attack Reset

	// STATE MACHINES
	StateMachine::FStateMachine Sequence1; // Sequence 1 state machine - performs boss' 1st sequence
	StateMachine::FStateMachine Sequence2; // Sequence 2 state machine - performs boss' 2nd sequence
	StateMachine::FStateMachine Sequence3; // Sequence 3 state machine - performs boss' 3rd sequence
#pragma endregion 

#pragma region SetStates
	
	// STATES - Sequence 1
	// Starting boot up - add transition(s) to array
	StartingBootUpState->index = 6; // boot up
	StartingBootUpState->NextStates.Add(IdleSeq1State); // Transition to idle

	// Idle seq 1 - add transition(s) to array 
	IdleSeq1State->index = 0; // idle
	IdleSeq1State->NextStates.Add(LaserSeq1State);
	
	// Laser seq 1 - add transition(s) to array 
	LaserSeq1State->index = 1; // laser
	LaserSeq1State->NextStates.Add(IdleSeq1State);

	
	// STATES - Sequence 2
	// shield up seq 2 - add transition(s) to array
	ShieldUpSeq2->index = 7; // gen shield up
	ShieldUpSeq2->NextStates.Add(ParkourModeState);
	
	// parkour mode seq 2 - add transition(s) to array 
	ParkourModeState->index = 5; // parkour
	ParkourModeState->NextStates.Add(IdleSeq2State);

	// Idle seq 2 - add transition(s) to array 
	IdleSeq2State->index = 0; // idle
	IdleSeq2State->NextStates.Add(ObjDropSeq2State);
	
	// ObjDrop seq 2 - add transition(s) to array 
	ObjDropSeq2State->index = 3; // obj drop attack
	ObjDropSeq2State->NextStates.Add(ObjDropResetSeq2State);

	// ObjDrop reset seq 2 - add transition(s) to array 
	ObjDropResetSeq2State->index = 4; // obj drop attack reset
	ObjDropResetSeq2State->NextStates.Add(IdleSeq2State);

	
	// STATES - Sequence 3
	// personal shield up seq 3 - add transition(s) to array 
	PersonalShieldSeq3->index = 8; // personal shield up
	PersonalShieldSeq3->NextStates.Add(IdleSeq3State);

	// Idle seq 3 - add transition(s) to array 
	IdleSeq3State->index = 9; // idle seq 3
	IdleSeq3State->NextStates.Add(ObjDropSeq3State);
	IdleSeq3State->NextStates.Add(DoubleLaserSeq3State);
	
	// Laser seq 3 - add transition(s) to array 
	DoubleLaserSeq3State->index = 2; // double laser
	DoubleLaserSeq3State->NextStates.Add(IdleSeq3State);

	// Fist attack seq 3 - add transition(s) to array 
	ObjDropSeq3State->index = 3; // obj drop attack
	ObjDropSeq3State->NextStates.Add(ObjDropResetSeq3State);

	// Fist reset seq 3 - add transition(s) to array 
	ObjDropResetSeq3State->index = 4; // obj drop attack reset;
	ObjDropResetSeq3State->NextStates.Add(IdleSeq3State);
	
#pragma endregion	

#pragma region SetStateMachines
	
	// STATESMACHINES
	// Sequence 1 - Set states
	Sequence1.States.Add(StartingBootUpState);
	Sequence1.States.Add(IdleSeq1State);
	Sequence1.States.Add(LaserSeq1State);
	Sequence1.CurrentState = StartingBootUpState;

	// Sequence 2 - Set States
	Sequence2.States.Add(ShieldUpSeq2);
	Sequence2.States.Add(ParkourModeState);
	Sequence2.States.Add(IdleSeq2State);
	Sequence2.States.Add(ObjDropSeq2State);
	Sequence2.States.Add(ObjDropResetSeq2State);
	Sequence2.CurrentState = ObjDropSeq2State;

	// Sequence 3 - Set States
	Sequence3.States.Add(PersonalShieldSeq3);
	Sequence3.States.Add(IdleSeq3State);
	Sequence3.States.Add(DoubleLaserSeq3State);
	Sequence3.States.Add(ObjDropSeq3State);
	Sequence3.States.Add(ObjDropResetSeq3State);
	Sequence3.CurrentState = PersonalShieldSeq3;

	StateMachines.Add(Sequence1);
	StateMachines.Add(Sequence2);
	StateMachines.Add(Sequence3);
	
#pragma endregion

	FunctionPtrs.Add(&ABoss::Idle);
	FunctionPtrs.Add(&ABoss::Lasers);
	FunctionPtrs.Add(&ABoss::DoubleLaser);
	FunctionPtrs.Add(&ABoss::ObjDropAttack);
	FunctionPtrs.Add(&ABoss::ObjDropAttackReset);
	FunctionPtrs.Add(&ABoss::Parkour);
	FunctionPtrs.Add(&ABoss::StartBootUp);
	FunctionPtrs.Add(&ABoss::GenShieldUp);
	FunctionPtrs.Add(&ABoss::PersonalShieldUp);
	FunctionPtrs.Add(&ABoss::IdleSeq3);
	
	
}

void ABoss::Idle(float DeltaTime)
{
	if (!bIdleTimerStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle"));
		
		GetWorld()->GetTimerManager().SetTimer(IdleHandle, FTimerDelegate::CreateLambda([&]
		{
			GetWorld()->GetTimerManager().ClearTimer(IdleHandle);
			UE_LOG(LogTemp, Warning, TEXT("idle timer done"));
			bIdleTimerStarted = false;
			StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
		}), FMath::RandRange(1.5f, 3.0f), false);

		bIdleTimerStarted = true;
	}
}

/**
 * @brief Laser attack for the boss
 */
void ABoss::Lasers(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Laser pew pew"));

	if (!bLaserStarted)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("LaserLeft");
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABoss::LasersAnimNotify);
		bLaserStarted = true;
	}

	if (bLaserSpawned && bLeftLaser)
	{
		Laser->SetActorLocation(GetMesh()->GetSocketLocation("L_LaserSocket"));
		Laser->SetActorRotation(GetMesh()->GetSocketRotation("L_LaserSocket"));
	}
	else if (bLaserSpawned && !bLeftLaser)
	{
		Laser->SetActorLocation(GetMesh()->GetSocketLocation("R_LaserSocket"));
		Laser->SetActorRotation(GetMesh()->GetSocketRotation("R_LaserSocket"));
	}
}

/**
 * @brief Resets laser to original position & rotation
 */
 void ABoss::DoubleLaser(float DeltaTime)
{
	if (!bDoubleLaserStarted)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection("DoubleLaser");
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABoss::DoubleLasersAnimNotify);
		bDoubleLaserStarted = true;
	}

	if (bDoubleLaserStarted)
	{
		Laser->SetActorLocation(GetMesh()->GetSocketLocation("L_LaserSocket"));
		Laser->SetActorRotation(GetMesh()->GetSocketRotation("L_LaserSocket"));

		Laser->SetActorLocation(GetMesh()->GetSocketLocation("R_LaserSocket"));
		Laser->SetActorRotation(GetMesh()->GetSocketRotation("R_LaserSocket"));
	}
}

/**
 * @brief Fist attack for boss
 */
void ABoss::ObjDropAttack(float DeltaTime)
{
	FVector PlayerFeetLoc = FVector(PlayerRef->GetActorLocation().X, PlayerRef->GetActorLocation().Y, 0.0f);
	
	// If tracker hasnt spawned spawn it 
	if (!bTrackerSpawned)
	{
		Tracker = GetWorld()->SpawnActor<AActor>(TrackerClassRef, PlayerFeetLoc, FRotator::ZeroRotator);
		bTrackerSpawned = true;
	}

	if (Tracker != nullptr)
	{
		if (!GetWorldTimerManager().IsTimerActive(TrackerAttackHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(TrackerAttackHandle, FTimerDelegate::CreateLambda([&] { bTrackerAttackDone = true; UE_LOG(LogTemp, Warning, TEXT("timer done"));}), MaxTrackerTime, false);
			UE_LOG(LogTemp, Warning, TEXT("timer set")); // Condition
		}
		
		if (!bTrackerAttackDone)
		{
			Tracker->SetActorLocation(FMath::Lerp(Tracker->GetActorLocation(), PlayerFeetLoc, DeltaTime * TrackerSpeed));
		}
		else
		{
			if (!bObjSpawned)
			{
				GetWorld()->GetTimerManager().ClearTimer(TrackerAttackHandle);

				GetMesh()->GetAnimInstance()->Montage_JumpToSection("ObjectSpawn");
				AActor* HeavyObject = GetWorld()->SpawnActor<AActor>(ObjDroppedClassRef, FVector(Tracker->GetActorLocation().X, Tracker->GetActorLocation().Y, Tracker->GetActorLocation().Z + ObjectSpawnHeight), FRotator::ZeroRotator);

				if (HeavyObject)
				{
					bObjSpawned = true;
					StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
					UE_LOG(LogTemp, Warning, TEXT("Obj spawned")); // Condition
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Obj no spawn")); // Condition
				}
			}
		}
	}
}

/**
 * @brief Resets fist to original position & rotation
 */
void ABoss::ObjDropAttackReset(float DeltaTime)
{
	// start buffer timer
	// reset anim
	// next state
	bTrackerSpawned = false;
	bObjSpawned = false;
	
	UE_LOG(LogTemp, Warning, TEXT("attac reset :)"));
	StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
}

/**
 * @brief Parkour segment
 */
void ABoss::Parkour(float DeltaTime)
{
}

void ABoss::StartBootUp(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Boot up"));
	StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
}

void ABoss::GenShieldUp(float DeltaTime)
{
}

void ABoss::PersonalShieldUp(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("personal shield up"));
	StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
}

void ABoss::IdleSeq3(float DeltaTime)
{
	if (!bIdleSeq3TimerStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle seq 3"));
		
		GetWorld()->GetTimerManager().SetTimer(IdleHandle, FTimerDelegate::CreateLambda([&]
		{
			GetWorld()->GetTimerManager().ClearTimer(IdleHandle);
			UE_LOG(LogTemp, Warning, TEXT("idle seq 3 timer done"));
			bIdleSeq3TimerStarted = false;
			StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[FMath::RandRange(0,1)];
		}), FMath::RandRange(1.5f, 3.0f), false);

		bIdleSeq3TimerStarted = true;
	}
}

void ABoss::LasersAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "SpawnLaserLeft")
	{
		if (!bLaserSpawned)
		{
			Laser = GetWorld()->SpawnActor<AActor>(LaserClassRef, GetMesh()->GetSocketLocation("L_LaserSocket") , GetMesh()->GetSocketRotation("L_LaserSocket"));
			bLeftLaser = true;
				
			if (Laser)
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser left spawned")); // Condition
				Laser->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,  false), "L_LaserSocket");
				bLaserSpawned = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser lefty no spawned")); // Condition
			}
		}
	}
	else if (NotifyName == "SpawnLaserRight")
	{
		if (!bLaserSpawned)
		{
			Laser = GetWorld()->SpawnActor<AActor>(LaserClassRef, GetMesh()->GetSocketLocation("R_LaserSocket") , GetMesh()->GetSocketRotation("R_LaserSocket"));

			if (Laser)
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser right spawned")); // Condition
				Laser->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,  false), "R_LaserSocket");
				bLaserSpawned = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser right no spawned")); // Condition
			}
		}
	}
	else if (NotifyName == "DeleteLaser")
	{
		if (Laser)
		{
			bLaserSpawned = false;
			bLeftLaser = false;
			UE_LOG(LogTemp, Warning, TEXT("Laser bye bye")); // Condition
			Laser->Destroy();
			Laser = nullptr;
		}
	}
	else if (NotifyName == "LasersDone")
	{
		UE_LOG(LogTemp, Warning, TEXT("Switching to next state LASERSDONE"));
		bLaserStarted = false;
		StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
	}
}

void ABoss::DoubleLasersAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//DoubleLasersDone DeleteDoubleLaser SpawnDoubleLaser

	if (NotifyName == "DoubleLasersDone")
	{
		if (!bLaserSpawned)
		{
			DoubleLaserL = GetWorld()->SpawnActor<AActor>(LaserClassRef, GetMesh()->GetSocketLocation("L_LaserSocket") , GetMesh()->GetSocketRotation("L_LaserSocket"));
			DoubleLaserR = GetWorld()->SpawnActor<AActor>(LaserClassRef, GetMesh()->GetSocketLocation("R_LaserSocket") , GetMesh()->GetSocketRotation("R_LaserSocket"));
				
			if (DoubleLaserL && DoubleLaserR)
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser double spawned")); // Condition
				DoubleLaserL->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,  false), "L_LaserSocket");
				DoubleLaserR->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,  false), "R_LaserSocket");
				bDoubleLaserSpawned = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("double Laser no spawned")); // Condition
			}
		}
	}
	else if (NotifyName == "DeleteDoubleLaser")
	{
		if (Laser)
		{
			bDoubleLaserSpawned = false;
			UE_LOG(LogTemp, Warning, TEXT("double Laser bye bye")); // Condition
			DoubleLaserL->Destroy();
			DoubleLaserR->Destroy();
			DoubleLaserL = nullptr;
			DoubleLaserR = nullptr;
		}
	}
	else if (NotifyName == "SpawnDoubleLaser")
	{
		UE_LOG(LogTemp, Warning, TEXT("double lasers done switched states"));
		bDoubleLaserStarted = false;
		StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
	}
}
