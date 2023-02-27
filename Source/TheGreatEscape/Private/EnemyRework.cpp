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
#include "EnemyReworkController.h"

// Sets default values
AEnemyRework::AEnemyRework()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetUpStimulus();

	AbilitySystemComponent = CreateDefaultSubobject<UQRAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UQRAttributeSet>(TEXT("Attributes"));
	
}

void AEnemyRework::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
	AQRCharacter* InstigatorCharacter, AActor* DamagerCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamagerCauser); 
}

void AEnemyRework::HandleHealthChanged(float Deltavalue, const FGameplayTagContainer& EventTags)
{
	if(bAbilitiesInitalized)
	{
		OnHealthChanged(Deltavalue, EventTags); 
	}	
}

void AEnemyRework::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AEnemyReworkController* EnemyController = Cast<AEnemyReworkController>(NewController);

	if (EnemyController)
	{
		EnemyController->EEnemyType = Utilities::EnemyTypes::Melee;
		UE_LOG(LogTemp, Warning, TEXT("enemy type set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyController cast failed"));
	}
	
	//Server Gas Init
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	AddStartupGameplayAbilities();
}

void AEnemyRework::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Server Gas Init
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID",
			static_cast<int32>(EGASAbilityInputID::Confirm),static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AEnemyRework::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent);
	if(GetLocalRole() == ROLE_Authority && !bAbilitiesInitalized)
	{
		//Grant Abilities, but only on Server
		for(TSubclassOf<UQRGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1,
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}

	for (const TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);

		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get(), AbilitySystemComponent);
		}

		bAbilitiesInitalized = true;
	}
}

UAbilitySystemComponent* AEnemyRework::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

//ANPCPatrolPath* AEnemyRework::GetPatrolPath()
//{
	//return PatrolPath;
//}

void AEnemyRework::Attack(UAbilitySystemComponent* TargetActorASC)
{
	if (TargetActorASC)
	{
		//TargetActorASC->ApplyGameplayEffectToTarget(DamageEffect, TargetActorASC, 1, FGameplayEffectContextHandle);
	}
	
	// Attack code
	//UE_LOG(LogTemp, Warning, TEXT("Attack function called"));
}

void AEnemyRework::SetUpStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

