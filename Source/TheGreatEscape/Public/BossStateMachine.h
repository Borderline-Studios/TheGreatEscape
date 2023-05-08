// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : Utils.h
// Description : Lists blackboard keys so retyping is not necessary, also some other Util stuff for other classes
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

/**
 * @brief Holds the structs I need for my own state machine
 */
namespace StateMachine
{
	struct FTransition; // forward declaring FTransition
	
	// State, holds the transition for the state and a delegate to the function it performs
	struct FState
	{
		TArray<FState*> NextStates;
		int index;
	};

	// State machine, holds all the states & the current state
	struct FStateMachine
	{
		TArray<FState*> States;
		FState* CurrentState;
	};
	
}
