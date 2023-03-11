// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkController.cpp
// Description : Custom ai controller
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "EnemyReworkController.h"

#include "EnemySpawner.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviourTree/BlackboardKeys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Character/Player/PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"

// Set up static array
TArray<UBehaviorTree*> AEnemyReworkController::BehaviorTreeReferences;

/**
 * @brief constructor, set up references
 * @param ObjectInitializer  Finalise creation after c++ constructor is called
 */
AEnemyReworkController::AEnemyReworkController(FObjectInitializer const& ObjectInitializer)
{
	// MELEE ENEMY TREE SET UP
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>objMelee(TEXT("BehaviorTree'/Game/Production/Enemies/Rework/BT_EnemyRework.BT_EnemyRework'"));

	// If behaviour tree found, set it
	if (objMelee.Succeeded())
	{
		BehaviorTreeReferences.Push(objMelee.Object);
		UE_LOG(LogTemp, Warning, TEXT("BT set"));
	}

	// DRONE ENEMY TREE SET UP
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>objDrone(TEXT("BehaviorTree'/Game/Production/Enemies/Rework/BT_EnemyReworkDrone.BT_EnemyReworkDrone'"));

	// If behaviour tree found, set it
	if (objDrone.Succeeded())
	{
		BehaviorTreeReferences.Push(objDrone.Object);
		UE_LOG(LogTemp, Warning, TEXT("Drone bt set"));
	}

	// HYBRID ENEMY TREE SET UP
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>objHybrid(TEXT("BehaviorTree'/Game/Production/Enemies/Rework/BT_EnemyReworkHybrid.BT_EnemyReworkHybrid'"));

	// If behaviour tree found, set it
	if (objHybrid.Succeeded())
	{
		BehaviorTreeReferences.Push(objHybrid.Object);
		UE_LOG(LogTemp, Warning, TEXT("Hybrid bt set"));
	}

	// NPC TREE SET UP
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>objNPC(TEXT("BehaviorTree'/Game/Production/NPCs/BT_NPC.BT_NPC'"));

	// If behaviour tree found, set it
	if (objNPC.Succeeded())
	{
		BehaviorTreeReferences.Push(objNPC.Object);
		UE_LOG(LogTemp, Warning, TEXT("NPC bt set"));
	}

	// initalise the behaviour tree comp and blackboard
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviourComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	// call set up for perception system
	SetupPerceptionSystem();
}

/**
 * @brief Runs & starts the behaviour tree 
 */
void AEnemyReworkController::BeginPlay()
{
	Super::BeginPlay();

	
	
}

/**
 * @brief init the blackboard
 * @param InPawn pawn its possessing
 */
void AEnemyReworkController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree != nullptr)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("behaviour tree no run no start brrrr"));
	}
	
	// if blackboard is not nullptr init it
	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

/**
 * @brief returns blackboard
 * @return blackboard
 */
UBlackboardComponent* AEnemyReworkController::GetBlackboard() const
{
	return Blackboard;
}

/**
 * @brief Sets up the behaviour tree depending on what type of enemy it is
 * @param EEnemyType Takes in what type of enemy it is
 */
void AEnemyReworkController::SetBehaviourTree(Utilities::EnemyTypes EEnemyType)
{
	// check all enemy types
	switch (EEnemyType)
	{
	case Utilities::EnemyTypes::Melee: // MELEE
		{
			BehaviorTree = BehaviorTreeReferences[0];
			//UE_LOG(LogTemp, Warning, TEXT("MELEE %s"), *BehaviorTree->GetName());
			break;
		}
	case Utilities::EnemyTypes::Drone: // DRONE
		{
			BehaviorTree = BehaviorTreeReferences[1];
			//UE_LOG(LogTemp, Warning, TEXT("DRONE"));
			break;
		}
	case Utilities::EnemyTypes::Hybrid: // Hybrid
		{
			BehaviorTree = BehaviorTreeReferences[2];
			//UE_LOG(LogTemp, Warning, TEXT("Hybrid"));
			break;
		}
	case Utilities::EnemyTypes::NPC: // NPC
		{
			BehaviorTree = BehaviorTreeReferences[3];
			//UE_LOG(LogTemp, Warning, TEXT("NPC"));
			break;
		}
	}
}

/**
 * @brief When the target becomes seen
 * @param actor actor seen
 * @param stimulus what sense has been used
 */
void AEnemyReworkController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	// check if seen actor is the player
	auto const character = Cast<APlayerCharacter>(actor);

	// if it is, change a bool in the blackboard
	if (character)
	{
		GetBlackboard()->SetValueAsBool(BbKeys::canSeePlayer, stimulus.WasSuccessfullySensed());
	}

}

/**
 * @brief Sets up the perception system
 */
void AEnemyReworkController::SetupPerceptionSystem()
{
	// Create and initalises sight config object
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Conifg"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	SightConfig->SightRadius = 2000.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 500.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->SetMaxAge(0.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.0f;
	SightConfig->PointOfViewBackwardOffset = 200.0f;
	SightConfig->NearClippingRadius = 200.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// ADd sight config component to perception component
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyReworkController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
