// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyRework.cpp
// Description : Dashing Vairent
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "EnemyReworkDasher.h"

#include "EnemyReworkController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AEnemyReworkDasher::AEnemyReworkDasher()
{
	// constructor
}

void AEnemyReworkDasher::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AEnemyReworkDasher::BeginPlay()
{
	Super::BeginPlay();
	
	// Set is Dasher varient
	if (const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(GetController()))
	{
		AIController->GetBlackboard()->SetValueAsBool(BbKeys::isMelee, false);
	}

	GetMesh()->OnComponentHit.AddDynamic(this, &AEnemyReworkDasher::OnHit);

	ControllerRef = Cast<AEnemyReworkController>(GetController());

	DefaultMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AEnemyReworkDasher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyReworkDasher::PostDeathProcess()
{
	Super::PostDeathProcess();
}

void AEnemyReworkDasher::PostHitProcess()
{
	Super::PostHitProcess();
}

void AEnemyReworkDasher::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ControllerRef) // checks the controller ref is populated
	{
		if(ControllerRef->GetBlackboard()->GetValueAsBool(BbKeys::isDashing)) // if enemy is dashing check collisions
		{
			// check if collided with player
			if (APlayerCharacter* otherActor = Cast<APlayerCharacter>(OtherActor))
			{
				UAbilitySystemComponent* ASC = otherActor->GetAbilitySystemComponent();
				if (ASC)
				{
					FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
					EffectContext.AddSourceObject(otherActor);
			
					FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
					ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), ASC);
					otherActor->PostHitProcess();
					UE_LOG(LogTemp, Warning, TEXT("Player dmg dash"));
				}
			}
			
			StopDash(); // stops the dash
		}
	}
}

void AEnemyReworkDasher::StopDash()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
	ControllerRef->StopMovement();
	GetMesh()->GetAnimInstance()->Montage_JumpToSection("DashEnd");
	ControllerRef->GetBlackboard()->SetValueAsBool(BbKeys::isDashing, false);
	ControllerRef->GetBlackboard()->SetValueAsBool(BbKeys::inDashingState, false);
}
