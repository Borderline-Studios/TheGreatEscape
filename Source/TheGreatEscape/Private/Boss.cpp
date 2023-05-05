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
	StateMachine::FTransition StartingBootUpTransition; // Sequence 1 - Starting animation transition
	StateMachine::FTransition IdleSeq1Transition; // Sequence 1 - Idle  transition
	StateMachine::FTransition LaserSeq1Transition; // Sequence 1 - Laser transition

	StateMachine::FTransition ShieldUpSeq2Transition; // Sequence 2 - Shield up transition 
	StateMachine::FTransition ParkourModeTransition; // Sequence 2 - Parkour transition
	StateMachine::FTransition IdleSeq2Transition; // Sequence 2 - Idle  transition
	StateMachine::FTransition ObjDropSeq2Transition; // Sequence 2 - ObjDrop Attack transition
	StateMachine::FTransition ObjDropResetSeq2Transition; // Sequence 2 - ObjDrop Attack Reset transition 

	StateMachine::FTransition PersonalShieldSeq3Transition; // Sequence 3 - Personal Shield up transition
	StateMachine::FTransition IdleSeq3Transition1; // Sequence 3 - Idle  transition 1 (to double laser)
	StateMachine::FTransition IdleSeq3Transition2; // Sequence 3 - Idle  transition 2 (to obj drop)
	StateMachine::FTransition DoubleLaserSeq3Transition; // Sequence 3 - Double Laser transition
	StateMachine::FTransition ObjDropSeq3Transition; // Sequence 3 - Fist Attack 
	StateMachine::FTransition ObjDropResetSeq3Transition; // Sequence 3 - Fist Attack Reset transition 1 
	
	// STATES
	StateMachine::FState StartingBootUpState; // Sequence 1 - Starting animation 
	StateMachine::FState IdleSeq1State; // Sequence 1 - Idle
	StateMachine::FState LaserSeq1State; // Sequence 1 - Laser 

	StateMachine::FState ShieldUpSeq2; // Sequence 2 - Generated shield up
	StateMachine::FState ParkourModeState; // Sequence 2 - Parkour
	StateMachine::FState IdleSeq2State; // Sequence 2 - Idle
	StateMachine::FState ObjDropSeq2State; // Sequence 2 - Fist Attack 
	StateMachine::FState ObjDropResetSeq2State; // Sequence 2 - Fist Attack Reset

	StateMachine::FState PersonalShieldSeq3; // Sequence 3 - Personal shield up
	StateMachine::FState IdleSeq3State; // Sequence 3 - Idle
	StateMachine::FState DoubleLaserSeq3State; // Sequence 3 - Double Laser
	StateMachine::FState ObjDropSeq3State; // Sequence 3 - ObjDrop Attack 
	StateMachine::FState ObjDropResetSeq3State; // Sequence 3 - Fist Attack Reset

	// STATE MACHINES
	StateMachine::FStateMachine Sequence1; // Sequence 1 state machine - performs boss' 1st sequence
	StateMachine::FStateMachine Sequence2; // Sequence 2 state machine - performs boss' 2nd sequence
	StateMachine::FStateMachine Sequence3; // Sequence 3 state machine - performs boss' 3rd sequence
#pragma endregion 

#pragma region SetTransitions
	
	// TRANSITIONS - Sequence 1

	// Starting Boot up Condition & NextState
	StartingBootUpTransition.ConditionDelegate.BindLambda([&]()->bool
	{
		UE_LOG(LogTemp, Warning, TEXT("StartingBootUpTransition lambda called")); // Condition

		//temp = true;
		return true;
	});
	StartingBootUpTransition.NextState = IdleSeq1State; // To idle (seq 1)

	// Idle Condition & NextState
	IdleSeq1Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//(LogTemp, Warning, TEXT("IdleSeq1Transition lambda called")); // Condition

		//temp = true;
		return true;
	});
	IdleSeq1Transition.NextState = LaserSeq1State; // To laser  (seq 1)
	
	// Laser Condition & NextState
	LaserSeq1Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		UE_LOG(LogTemp, Warning, TEXT("LaserSeq1Transition lambda called")); // Condition

		//temp = true;
		return true;
	});
	LaserSeq1Transition.NextState = IdleSeq1State; // To idle (seq 1)
	

	
	// TRANSITIONS - Sequence 2

	// Shield up Condition & NextState
	ShieldUpSeq2Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		UE_LOG(LogTemp, Warning, TEXT("ShieldUpSeq2Transition lambda called")); // Condition

		//temp = true;
		return true;
	});
	ShieldUpSeq2Transition.NextState = ParkourModeState; // To parkour (seq 2)
	
	// Parkour condition & next state
	ParkourModeTransition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded ParkourModeTransition Condition lamda function")); // Condition
		return true;
	});
	ParkourModeTransition.NextState = IdleSeq2State; // To Idle (seq 2)

	// Idle Condition & NextState
	IdleSeq2Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//(LogTemp, Warning, TEXT("IdleSeq2Transition lambda called")); // Condition

		//temp = true;
		return true;
	});
	IdleSeq2Transition.NextState = ObjDropSeq2State; // To Obj drop attack  (seq 2)

	// obj drop seq 2 Condition & next state
	ObjDropSeq2Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded ObjDropSeq2Transition Condition lamda function")); // Condition
		return true;
	});
	ObjDropSeq2Transition.NextState = ObjDropResetSeq2State; // To obj drop reset (seq 2)

	// Fist reset seq 2 condition & next state: Transition 1 -> Parkour
	ObjDropResetSeq2Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded ObjDropResetSeq2Transition Condition lamda function")); // Condition
		return true;
	});
	ObjDropResetSeq2Transition.NextState = IdleSeq2State; // To Idle (seq 2)
	


	// TRANSITIONS - Sequence 3

	// personal shield Condition & NextState
	PersonalShieldSeq3Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("PersonalShieldSeq3Transition lambda called")); // Condition

		//temp = true;
		return true;
	});
	PersonalShieldSeq3Transition.NextState = IdleSeq3State; // To idle (seq 3)

	// Idle Condition & NextState
	IdleSeq3Transition1.ConditionDelegate.BindLambda([&]()->bool
	{
		//(LogTemp, Warning, TEXT("IdleSeq3Transition1 lambda called")); // Condition

		//temp = true;
		return true;
	});
	IdleSeq3Transition1.NextState = DoubleLaserSeq3State; // To double laser  (seq 3)

	// Idle Condition & NextState
	IdleSeq3Transition2.ConditionDelegate.BindLambda([&]()->bool
	{
		//(LogTemp, Warning, TEXT("IdleSeq3Transition2 lambda called")); // Condition

		//temp = true;
		return true;
	});
	IdleSeq3Transition2.NextState = ObjDropSeq3State; // To obj drop  (seq 3)
	
	// Double Laser seq 3 condition & next state
	DoubleLaserSeq3Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded LaserSeq3Transition Condition lamda function")); // Condition
		return true;
	});
	DoubleLaserSeq3Transition.NextState = IdleSeq3State; // to idle (seq 3)
	
	// obj drop seq 3 condition & next state
	ObjDropSeq3Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded FistSeq3Transition Condition lamda function")); // Condition
		return true;
	});
	ObjDropSeq3Transition.NextState = ObjDropResetSeq3State; // to obj drop reset (seq 3)

	// fist reset seq 3 condition & next state: Transition 1 -> laser 
	ObjDropResetSeq3Transition.ConditionDelegate.BindLambda([&]()->bool
	{
		//UE_LOG(LogTemp, Warning, TEXT("Binded FistResetSeq3Transition1 Condition lamda function")); // Condition
		return true;
	});
	ObjDropResetSeq3Transition.NextState = IdleSeq3State; // to idle (seq 3)



#pragma endregion

#pragma region SetStates
	
	// STATES - Sequence 1
	// Starting boot up - add transition(s) to array & bind delegate function
	StartingBootUpState.Transitions.Add(StartingBootUpTransition);
	//StartingBootUpState.DelegateFunction.Add();

	// Idle seq 1 - add transition(s) to array & bind delegate function
	IdleSeq1State.Transitions.Add(IdleSeq1Transition);
	//IdleSeq1State.DelegateFunction.Add();
	
	// Laser seq 1 - add transition(s) to array & bind delegate function
	LaserSeq1State.Transitions.Add(LaserSeq1Transition);
	LaserSeq1State.DelegateFunction.BindUObject(this, &ABoss::Laser);

	
	// STATES - Sequence 2
	// shield up seq 2 - add transition(s) to array & bind delegate function
	ShieldUpSeq2.Transitions.Add(ShieldUpSeq2Transition);
	//ShieldUpSeq2.DelegateFunction.BindUObject(this, &ABoss::Parkour);
	
	// parkour mode seq 2 - add transition(s) to array & bind delegate function
	ParkourModeState.Transitions.Add(ParkourModeTransition);
	ParkourModeState.DelegateFunction.BindUObject(this, &ABoss::Parkour);

	// Idle seq 2 - add transition(s) to array & bind delegate function
	IdleSeq2State.Transitions.Add(IdleSeq2Transition);
	//IdleSeq2State.DelegateFunction.Add();
	
	// ObjDrop seq 2 - add transition(s) to array & bind delegate function
	ObjDropSeq2State.Transitions.Add(ObjDropSeq2Transition);
	ObjDropSeq2State.DelegateFunction.BindUObject(this, &ABoss::ObjDropAttack);

	// ObjDrop reset seq 2 - add transition(s) to array & bind delegate function
	ObjDropResetSeq2State.Transitions.Add(ObjDropResetSeq2Transition);
	ObjDropResetSeq2State.DelegateFunction.BindUObject(this, &ABoss::ObjDropAttackReset);

	
	// STATES - Sequence 3
	// personal shield up seq 3 - add transition(s) to array & bind delegate function
	PersonalShieldSeq3.Transitions.Add(PersonalShieldSeq3Transition);
	//PersonalShieldSeq3.DelegateFunction.BindUObject(this, &ABoss::Parkour);

	// Idle seq 3 - add transition(s) to array & bind delegate function
	IdleSeq3State.Transitions.Add(IdleSeq3Transition1);
	IdleSeq3State.Transitions.Add(IdleSeq3Transition2);
	//IdleSeq3State.DelegateFunction.Add();
	
	// Laser seq 3 - add transition(s) to array & bind delegate function
	DoubleLaserSeq3State.Transitions.Add(DoubleLaserSeq3Transition);
	DoubleLaserSeq3State.DelegateFunction.BindUObject(this, &ABoss::DoubleLaser);

	// Fist attack seq 3 - add transition(s) to array & bind delegate function
	ObjDropSeq3State.Transitions.Add(ObjDropSeq3Transition);
	ObjDropSeq3State.DelegateFunction.BindUObject(this, &ABoss::ObjDropAttack);

	// Fist reset seq 3 - add transition(s) to array & bind delegate function
	ObjDropResetSeq3State.Transitions.Add(ObjDropResetSeq3Transition);
	ObjDropResetSeq3State.DelegateFunction.BindUObject(this, &ABoss::ObjDropAttackReset);
	
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
	Sequence2.CurrentState = ShieldUpSeq2;

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
 void ABoss::DoubleLaser()
{
	//UE_LOG(LogTemp, Warning, TEXT("Resetting laser"));
}

/**
 * @brief Fist attack for boss
 */
void ABoss::ObjDropAttack()
{
}

/**
 * @brief Resets fist to original position & rotation
 */
void ABoss::ObjDropAttackReset()
{
}

/**
 * @brief Parkour segment
 */
void ABoss::Parkour()
{
}
