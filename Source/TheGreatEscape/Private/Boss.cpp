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
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// INDEX IS UPDATED WHEN DAMAGED
	StateMachines[currentStateMachineIndex].CurrentState.DelegateFunction.Execute();

	for (int i = 0; i < StateMachines[currentStateMachineIndex].CurrentState.Transitions.Num(); i++)
	{
		if (StateMachines[currentStateMachineIndex].CurrentState.Transitions[i].ConditionDelegate.Execute())
		{
			//StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState.Transitions[i].NextState;
		}
		// if any condition returns true go to next state 
	}
}

void ABoss::StateMachineSetUps()
{
#pragma region CreateVariables
	// TRANSITIONS
	StateMachine::FTransition LaserSeq1Transition; // Sequence 1 - Laser transition
	StateMachine::FTransition LaserResetSeq1Transition; // Sequence 1 - Laser Reset transition

	StateMachine::FTransition ParkourModeTransition; // Sequence 2 - Parkour transition 
	StateMachine::FTransition FistSeq2Transition; // Sequence 2 - Fist Attack transition
	StateMachine::FTransition FistResetSeq2Transition1; // Sequence 2 - Fist Attack Reset transition 1 (to parkour)
	StateMachine::FTransition FistResetSeq2Transition2; // Sequence 2 - Fist Attack Reset transition 2 (to fist)
	
	StateMachine::FTransition LaserSeq3Transition; // Sequence 3 - Laser transition
	StateMachine::FTransition LaserResetSeq3Transition1; // Sequence 3 - Laser Reset transition 1 (to fist)
	StateMachine::FTransition LaserResetSeq3Transition2; // Sequence 3 - Laser Reset transition 2 (to laser)
	StateMachine::FTransition FistSeq3Transition; // Sequence 3 - Fist Attack 
	StateMachine::FTransition FistResetSeq3Transition1; // Sequence 3 - Fist Attack Reset transition 1 (to laser)
	StateMachine::FTransition FistResetSeq3Transition2; // Sequence 3 - Fist Attack Reset transition 2 (to fist)
	
	// STATES
	StateMachine::FState LaserSeq1State; // Sequence 1 - Laser
	StateMachine::FState LaserResetSeq1State; // Sequence 1 - Laser Resets

	StateMachine::FState ParkourModeState; // Sequence 2 - Parkour
	StateMachine::FState FistSeq2State; // Sequence 2 - Fist Attack 
	StateMachine::FState FistResetSeq2State; // Sequence 2 - Fist Attack Reset
	
	StateMachine::FState LaserSeq3State; // Sequence 3 - Laser
	StateMachine::FState LaserResetSeq3State; // Sequence 3 - Laser Reset
	StateMachine::FState FistSeq3State; // Sequence 3 - Fist Attack 
	StateMachine::FState FistResetSeq3State; // Sequence 3 - Fist Attack Reset

	// STATE MACHINES
	StateMachine::FStateMachine Sequence1; // Sequence 1 state machine - performs boss' 1st sequence
	StateMachine::FStateMachine Sequence2; // Sequence 2 state machine - performs boss' 2nd sequence
	StateMachine::FStateMachine Sequence3; // Sequence 3 state machine - performs boss' 3rd sequence
#pragma endregion 

#pragma region SetTransitions
	
	// TRANSITIONS - Sequence 1
	
	// Laser Condition & NextState
	LaserSeq1Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		UE_LOG(LogTemp, Warning, TEXT("LaserSeq1Transition lambda called")); // Condition

		//temp = true;
		return true;
	});
	LaserSeq1Transition.NextState = LaserResetSeq1State; // To laser reset (seq 1)

	// Laser Reset Condition & next state
	LaserResetSeq1Transition.ConditionDelegate.BindLambda([&]() ->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded LaserResetSeq1Transition Condition lamda function")); // Condition
		return true;
	});
	LaserResetSeq1Transition.NextState = LaserSeq1State; // To Laser (seq 1)

	
	// TRANSITIONS - Sequence 2
	
	// Parkour condition & next state
	ParkourModeTransition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded ParkourModeTransition Condition lamda function")); // Condition
		return true;
	});
	ParkourModeTransition.NextState = FistSeq2State; // To Fist attack (seq 2)

	// Fist seq 2 Condition & next state
	FistSeq2Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded FistSeq2Transition Condition lamda function")); // Condition
		return true;
	});
	FistSeq2Transition.NextState = FistResetSeq2State; // To fist reset (seq 2)

	// Fist reset seq 2 condition & next state: Transition 1 -> Parkour
	FistResetSeq2Transition1.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded FistResetSeq2Transition1 Condition lamda function")); // Condition
		return true;
	});
	FistResetSeq2Transition1.NextState = ParkourModeState; // To parkour (seq 2)

	// Fist reset seq 2 condition & next state: Transition 2 -> fist
	FistResetSeq2Transition2.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded FistResetSeq2Transition2 Condition lamda function")); // Condition
		return true;
	});
	FistResetSeq2Transition2.NextState = FistSeq2State; // to fist attack (seq 2)


	// TRANSITIONS - Sequence 3

	// Laser seq 3 condition & next state
	LaserSeq3Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded LaserSeq3Transition Condition lamda function")); // Condition
		return true;
	});
	LaserSeq3Transition.NextState = LaserResetSeq3State; // to laser reset (seq 3)

	// laser reset seq 3 condition & next state: Transition 1 -> fist
	LaserResetSeq3Transition1.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded LaserResetSeq3Transition1 Condition lamda function")); // Condition
		return true;
	});
	LaserResetSeq3Transition1.NextState = FistSeq3State; // to fist attack (seq 3)

	// laser reset seq 3 condition & next state: Transition 2 -> laser 
	LaserResetSeq3Transition2.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded LaserResetSeq3Transition2 Condition lamda function")); // Condition
		return true;
	});
	LaserResetSeq3Transition2.NextState = LaserSeq3State; // to laser (seq 3)

	// fist seq 3 condition & next state
	FistSeq3Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded FistSeq3Transition Condition lamda function")); // Condition
		return true;
	});
	FistSeq3Transition.NextState = FistResetSeq3State; // to fist reset (seq 3)

	// fist reset seq 3 condition & next state: Transition 1 -> laser 
	FistResetSeq3Transition1.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded FistResetSeq3Transition1 Condition lamda function")); // Condition
		return true;
	});
	FistResetSeq3Transition1.NextState = LaserSeq3State; // to laser (seq 3)

	// fist reset seq 3 condition & next state: Transition 2 -> fist  
	FistResetSeq3Transition2.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded FistResetSeq3Transition2 Condition lamda function")); // Condition
		return true;
	});
	FistResetSeq3Transition2.NextState = FistSeq3State; // to fist (seq 3)

#pragma endregion

#pragma region SetStates
	
	// STATES - Sequence 1
	// Laser seq 1 - add transition(s) to array & bind delegate function
	LaserSeq1State.Transitions.Add(LaserSeq1Transition);
	LaserSeq1State.DelegateFunction.BindUObject(this, &ABoss::Laser);

	// Laser reset seq 1 - add transition(s) to array & bind delegate function
	LaserResetSeq1State.Transitions.Add(LaserResetSeq1Transition);
	LaserResetSeq1State.DelegateFunction.BindUObject(this, &ABoss::LaserReset);

	
	// STATES - Sequence 2
	// parkour mode seq 2 - add transition(s) to array & bind delegate function
	ParkourModeState.Transitions.Add(ParkourModeTransition);
	ParkourModeState.DelegateFunction.BindUObject(this, &ABoss::Parkour);

	// Fist attack seq 2 - add transition(s) to array & bind delegate function
	FistSeq2State.Transitions.Add(FistSeq2Transition);
	FistSeq2State.DelegateFunction.BindUObject(this, &ABoss::Fist);

	// Fist reset seq 2 - add transition(s) to array & bind delegate function
	FistResetSeq2State.Transitions.Add(FistResetSeq2Transition1);
	FistResetSeq2State.Transitions.Add(FistResetSeq2Transition2);
	FistResetSeq2State.DelegateFunction.BindUObject(this, &ABoss::FistReset);

	
	// STATES - Sequence 3
	// Laser seq 3 - add transition(s) to array & bind delegate function
	LaserSeq3State.Transitions.Add(LaserSeq3Transition);
	LaserSeq3State.DelegateFunction.BindUObject(this, &ABoss::Laser);

	// Laser reset seq 3 - add transition(s) to array & bind delegate function
	LaserResetSeq3State.Transitions.Add(LaserResetSeq3Transition1);
	LaserResetSeq3State.Transitions.Add(LaserResetSeq3Transition2);
	LaserResetSeq3State.DelegateFunction.BindUObject(this, &ABoss::LaserReset);

	// Fist attack seq 3 - add transition(s) to array & bind delegate function
	FistSeq3State.Transitions.Add(FistSeq3Transition);
	FistSeq3State.DelegateFunction.BindUObject(this, &ABoss::Fist);

	// Fist reset seq 3 - add transition(s) to array & bind delegate function
	FistResetSeq3State.Transitions.Add(FistResetSeq3Transition1);
	FistResetSeq3State.Transitions.Add(FistResetSeq3Transition2);
	FistResetSeq3State.DelegateFunction.BindUObject(this, &ABoss::FistReset);
	
#pragma endregion	

#pragma region SetStateMachines
	
	// STATESMACHINES
	// Sequence 1 - Set states
	Sequence1.States.Add(LaserSeq1State);
	Sequence1.States.Add(LaserResetSeq1State);
	Sequence1.CurrentState = LaserSeq1State;

	// Sequence 2 - Set States
	Sequence2.States.Add(FistSeq2State);
	Sequence2.States.Add(FistResetSeq2State);
	Sequence2.States.Add(ParkourModeState);
	Sequence2.CurrentState = FistSeq2State;

	// Sequence 3 - Set States
	Sequence3.States.Add(LaserSeq3State);
	Sequence3.States.Add(LaserResetSeq3State);
	Sequence3.States.Add(FistSeq3State);
	Sequence3.States.Add(FistResetSeq3State);
	Sequence3.CurrentState = LaserSeq3State;


#pragma endregion

	StateMachines.Add(Sequence1);
	StateMachines.Add(Sequence2);
	StateMachines.Add(Sequence3);
}

/**
 * @brief Laser attack for the boss
 */
void ABoss::Laser()
{
	//UE_LOG(LogTemp, Warning, TEXT("Laser pew pew"));
}

/**
 * @brief Resets laser to original position & rotation
 */
 void ABoss::LaserReset()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resetting laser"));
}

/**
 * @brief Fist attack for boss
 */
void ABoss::Fist()
{
}

/**
 * @brief Resets fist to original position & rotation
 */
void ABoss::FistReset()
{
}

/**
 * @brief Parkour segment
 */
void ABoss::Parkour()
{
}
