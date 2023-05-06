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
	StartingBootUpState->index = 0; // temp
	StartingBootUpState->NextStates.Add(IdleSeq1State); // Transition to idle

	// Idle seq 1 - add transition(s) to array 
	IdleSeq1State->index = 0; // temp
	IdleSeq1State->NextStates.Add(LaserSeq1State);
	
	// Laser seq 1 - add transition(s) to array 
	LaserSeq1State->index = 1; // laser
	LaserSeq1State->NextStates.Add(IdleSeq1State);

	
	// STATES - Sequence 2
	// shield up seq 2 - add transition(s) to array
	ShieldUpSeq2->index = 0; // temp
	ShieldUpSeq2->NextStates.Add(ParkourModeState);
	
	// parkour mode seq 2 - add transition(s) to array 
	ParkourModeState->index = 5; // parkour
	ParkourModeState->NextStates.Add(IdleSeq2State);

	// Idle seq 2 - add transition(s) to array 
	IdleSeq2State->index = 0; // temp
	IdleSeq2State->NextStates.Add(ObjDropSeq2State);
	
	// ObjDrop seq 2 - add transition(s) to array 
	ObjDropSeq2State->index = 3; // obj drop attack
	ObjDropSeq2State->NextStates.Add(ObjDropResetSeq2State);

	// ObjDrop reset seq 2 - add transition(s) to array 
	ObjDropResetSeq2State->index = 4; // obj drop attack reset
	ObjDropResetSeq2State->NextStates.Add(IdleSeq2State);

	
	// STATES - Sequence 3
	// personal shield up seq 3 - add transition(s) to array 
	PersonalShieldSeq3->index = 0; // temp
	PersonalShieldSeq3->NextStates.Add(IdleSeq3State);

	// Idle seq 3 - add transition(s) to array 
	IdleSeq3State->index = 0; // temp
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

	FunctionPtrs.Add(&ABoss::Tempfunction);
	FunctionPtrs.Add(&ABoss::Laser);
	FunctionPtrs.Add(&ABoss::DoubleLaser);
	FunctionPtrs.Add(&ABoss::ObjDropAttack);
	FunctionPtrs.Add(&ABoss::ObjDropAttackReset);
	FunctionPtrs.Add(&ABoss::Parkour);
	
	
}

void ABoss::Tempfunction(float DeltaTime)
{
}

/**
 * @brief Laser attack for the boss
 */
void ABoss::Laser(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Laser pew pew"));
}

/**
 * @brief Resets laser to original position & rotation
 */
 void ABoss::DoubleLaser(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Resetting laser"));
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
			Tracker->SetActorLocation(FMath::Lerp(Tracker->GetActorLocation(), PlayerFeetLoc, DeltaTime));
		}
		else
		{
			if (!bObjSpawned)
			{
				GetWorld()->GetTimerManager().ClearTimer(TrackerAttackHandle);
				
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
		// start timer
		// follow player
		// when timer over drop the obj
		// move state
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
	UE_LOG(LogTemp, Warning, TEXT("attac reset :)"));
}

/**
 * @brief Parkour segment
 */
void ABoss::Parkour(float DeltaTime)
{
}
