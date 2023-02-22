// Fill out your copyright notice in the Description page of Project Settings.


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

// Called to bind functionality to input
void AEnemyRework::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

ANPCPatrolPath* AEnemyRework::GetPatrolPath()
{
	return PatrolPath;
}

void AEnemyRework::SetUpStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

