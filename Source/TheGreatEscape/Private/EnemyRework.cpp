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

#include "AbilitySystemBlueprintLibrary.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "EnemyReworkController.h"
#include "GeometryTypes.h"
#include "Math/UnrealMathUtility.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "DSP/MidiNoteQuantizer.h"
#include "BehaviorTree/BlackboardComponent.h"


/**
 * @brief constructor, sets up stimulus
 */
AEnemyRework::AEnemyRework()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetUpStimulus(); // call function to set up stimulus

	CalcRandomAttackPos(); // Find new target point

	// load death effect

	if (!DeathEffectReference)
	{
		DeathEffects = TSoftClassPtr<AEnemyRework>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Rework/BP_EnemyReworkDeath.BP_EnemyReworkDeath_C'")));
		DeathEffectReference = DeathEffects.LoadSynchronous();
	}
}

/**
 * @brief possessed by
 * @param NewController controller enemy is possessed by
 */
void AEnemyRework::PossessedBy(AController* NewController)
{
	// call super
	Super::PossessedBy(NewController);
	
	// get controller & if not empty if enemy
	if (AEnemyReworkController* EnemyController = Cast<AEnemyReworkController>(NewController))
	{
		// set tree and pass in melee enemy
		EnemyController->SetBehaviourTree(Utilities::EnemyTypes::Melee);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController cast failed melee"));
	}
}

/**
 * @brief Begin play, called when play started
 */
void AEnemyRework::BeginPlay()
{
	// call super
	Super::BeginPlay();

	// set is melee variant
	if (const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(GetController()))
	{
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::isMelee, true);
	}
	
}

/**
 * @brief Tick, called every frame
 * @param DeltaTime deltatime
 */
void AEnemyRework::Tick(float DeltaTime)
{
	// call super
	Super::Tick(DeltaTime);
}

/**
 * @brief Function to attack
 * @note not sure if function still needed
 */
void AEnemyRework::Attack()
{
	//TargetActorASC->ApplyGameplayEffectToTarget(DamageEffect, TargetActorASC, 1, FGameplayEffectContextHandle);
	// Attack code
	//UE_LOG(LogTemp, Warning, TEXT("Attack function called"));
}

/**
 * @brief Calculates an offset vector from train for the enemy to target 
 */
void AEnemyRework::CalcRandomAttackPos()
{
	// Get random X point from array
	int randIndex = FMath::RandRange(0, Utilities::XPointsLength - 1);
	float newXVal = Utilities::RandTrainXPoints[randIndex];

	// Random to make some on other side of train
	if (FMath::RandRange(0,1) == 0)
	{
		yOffsetFromTrain *= -1.0;
	}

	// set new vector
	TrainTargetPointOffset = FVector(newXVal, yOffsetFromTrain, ElevationHeight);
	//UE_LOG(LogTemp, Warning, TEXT("Target location for train: %s"), *TrainTargetPointOffset.ToString());
}

void AEnemyRework::PostDeathProcess()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSFX, this->GetActorLocation(), this->GetActorRotation(), 0.5, FMath::RandRange(0.7, 1.4));
	// Spawn in body parts
	FVector location =  this->GetActorLocation();
	AActor* deathEffect = Cast<AActor>(GetWorld()->SpawnActor(DeathEffectReference, &location));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, this->GetActorLocation(), this->GetActorRotation(), FVector(0.5,0.5,0.5), true);

}

void AEnemyRework::PostHitProcess()
{
	// if enemy dead
	if (CheckHealth() <= 0 && FirstDeath)
	{
		FirstDeath = false;
		PostDeathProcess();
		Destroy();
	}

	bool bFound;
	float Value = GetAbilitySystemComponent()->GetGameplayAttributeValue(UQRAttributeSet::GetShieldAttribute(), bFound);
	if (Value <= 0 && bFound)
	{
		ShieldBrakeChangeMaterial();
	}
}

float AEnemyRework::CheckHealth()
{
// get ability system component
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);

	// bool to check if it was found & the value the health equals
	bool bFound;
	float Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetHealthAttribute(), bFound);

	// if value found
	if (bFound)
	{
		return (Value);
	}

	return 0;
}

/**
 * @brief Set up stimulus
 */
void AEnemyRework::SetUpStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}


