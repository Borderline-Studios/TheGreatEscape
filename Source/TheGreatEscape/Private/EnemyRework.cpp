// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyRework.h
// Description : Parent Enemy class 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "EnemyRework.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"


// Sets default values
AEnemyRework::AEnemyRework()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetUpStimulus();
	
}

void AEnemyRework::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEnemyReworkController* EnemyController = Cast<AEnemyReworkController>(NewController);

	if (EnemyController)
	{
		//EnemyController->EEnemyType = Utilities::EnemyTypes::Melee;
		EnemyController->SetBehaviourTree(Utilities::EnemyTypes::Melee);
		//UE_LOG(LogTemp, Warning, TEXT("enemy type set melee"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController cast failed melee"));
	}
}

// Called when the game starts or when spawned
void AEnemyRework::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyRework::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//ANPCPatrolPath* AEnemyRework::GetPatrolPath()
//{
	//return PatrolPath;
//}

void AEnemyRework::Attack()
{
	
	//TargetActorASC->ApplyGameplayEffectToTarget(DamageEffect, TargetActorASC, 1, FGameplayEffectContextHandle);
	
	// Attack code
	//UE_LOG(LogTemp, Warning, TEXT("Attack function called"));
}

void AEnemyRework::SetUpStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

