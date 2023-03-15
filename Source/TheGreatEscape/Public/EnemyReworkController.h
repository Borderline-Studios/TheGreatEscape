// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkController.h
// Description : Custom ai controller
// Author      : Borderline Studios - Toni Natta
// Mail        : Toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviourTree/BlackboardKeys.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyReworkController.generated.h"

/**
 * Custom AI controller for the enemies & NPC
 */
UCLASS()
class THEGREATESCAPE_API AEnemyReworkController : public AAIController
{
	GENERATED_BODY()

public:
	// *** Functions *** ///
	AEnemyReworkController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get()); // constructor
	void BeginPlay() override; // begin play
	void OnPossess(APawn* const InPawn) override; // On possess
	class UBlackboardComponent* GetBlackboard() const; // Get the blackboard
	void SetBehaviourTree(Utilities::EnemyTypes EEnemyType); // Set the behaviour tree
		

protected:
	// *** Variables *** ///
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree; // Behaviour tree ref

	static TStaticArray<UBehaviorTree*, 4> BehaviorTreeReferences; // Array of different tree references

private:
	// *** Functions *** ///
	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const stimulus); // On target detected
	void SetupPerceptionSystem(); // Sets up the perception system

	// *** Variables *** ///
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent; // Behaviour tree component

	class UBlackboardComponent* Blackboard; // Blackboard

	class UAISenseConfig_Sight* SightConfig; // Sight sense config
	
	
};
