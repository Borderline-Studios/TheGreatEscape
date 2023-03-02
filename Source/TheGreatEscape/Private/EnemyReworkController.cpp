// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkController.cpp
// Description : Custom ai controller
// Author      :  Borderline Studios - Toni Natta
// Mail        :  toni.natta@mds.ac.nz


#include "EnemyReworkController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviourTree/BlackboardKeys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "Character/QRCharacter.h"
#include "Perception/AIPerceptionComponent.h"

TArray<UBehaviorTree*> AEnemyReworkController::BehaviorTreeReferences;

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

	// NPC TREE SET UP
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>objNPC(TEXT("BehaviorTree'/Game/Production/NPCs/BT_NPC.BT_NPC'"));

	// If behaviour tree found, set it
	if (objNPC.Succeeded())
	{
		BehaviorTreeReferences.Push(objNPC.Object);
		UE_LOG(LogTemp, Warning, TEXT("NPC bt set"));
	}
	
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviourComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	SetupPerceptionSystem();
}

void AEnemyReworkController::BeginPlay()
{
	Super::BeginPlay();
	
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AEnemyReworkController::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);

	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* AEnemyReworkController::GetBlackboard() const
{
	return Blackboard;
}

void AEnemyReworkController::SetBehaviourTree(Utilities::EnemyTypes EEnemyType)
{
	//AEnemyRework* Enemy = 
	// UGameplayStatics::GetActorOfClass(this, AQRCharacter::StaticClass())
	switch (EEnemyType)
	{
	case Utilities::EnemyTypes::Melee:
		{
			BehaviorTree = BehaviorTreeReferences[0];

			//UE_LOG(LogTemp, Warning, TEXT("MELEE"));
			
			break;
		}
	case Utilities::EnemyTypes::Drone:
		{
			BehaviorTree = BehaviorTreeReferences[1];
			
			//UE_LOG(LogTemp, Warning, TEXT("DRONE"));

			break;
		}
	case Utilities::EnemyTypes::NPC:
		{
			BehaviorTree = BehaviorTreeReferences[2];
			UE_LOG(LogTemp, Warning, TEXT("NPC"));
			break;
		}
	}
}

void AEnemyReworkController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("TARGET DETECTED"));

	auto const character = Cast<AQRCharacter>(actor);
	
	if (character)
	{
		GetBlackboard()->SetValueAsBool(BbKeys::canSeePlayer, stimulus.WasSuccessfullySensed());
		//UE_LOG(LogTemp, Warning, TEXT("sensed"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player things name: %s"), *actor->GetName());
	}
}

void AEnemyReworkController::SetupPerceptionSystem()
{
	// Create and initalise sight config object
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
