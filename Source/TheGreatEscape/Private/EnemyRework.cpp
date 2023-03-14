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


/**
 * @brief constructor, sets up stimulus
 */
AEnemyRework::AEnemyRework()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetUpStimulus(); // call function to set up stimulus
	
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
}

/**
 * @brief Tick, called every frame
 * @param DeltaTime deltatime
 */
void AEnemyRework::Tick(float DeltaTime)
{
	// call super
	Super::Tick(DeltaTime);

	// get ability system component
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);

	// bool to check if it was found & the value the health equals
	bool bFound;
	float Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetHealthAttribute(), bFound);

	// if value found
	if (bFound)
	{
		// if enemy dead
		if (Value <= 0)
		{
			// die
			UE_LOG(LogTemp, Warning, TEXT("die"));
			Destroy();
		}
	}
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
 * @brief Set up stimulus
 */
void AEnemyRework::SetUpStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

