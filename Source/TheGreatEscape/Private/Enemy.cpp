// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : Enemy.cpp
// Description : 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz



#include "Enemy.h"
#include "TheGreatEscape/TheGreatEscapeCharacter.h"
#include "Components/SphereComponent.h"

// AI includes
#include "EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "TheGreatEscape/TheGreatEscapeGameMode.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Player detection"));
	PlayerDetection->SetupAttachment(RootComponent);

	PlayerAttackDetection = CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Detection"));
	PlayerAttackDetection->SetupAttachment(RootComponent);

	HeadShotSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Headshot Sphere"));
	HeadShotSphere->SetupAttachment(RootComponent);

	MaxHealth = 20.0f;
	Health = MaxHealth;
	bDead = false;

	//GameMode = GetWorld()->GetAuthGameMode<ATheGreatEscapeGameMode>();
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<AEnemyAIController>(GetController());
	EnemyAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemy::OnAIMoveCompleted);


	PlayerDetection->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnPlayerDetectedOverlapBegin);
	PlayerDetection->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnPlayerDetectedOverlapEnd);

	PlayerAttackDetection->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnPlayerAttackOverlapBegin);
	PlayerAttackDetection->OnComponentEndOverlap.AddDynamic(this,&AEnemy::OnPlayerAttackOverlapEnd);

	HeadShotSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnPlayerAttackOverlapBegin);
	HeadShotSphere->OnComponentEndOverlap.AddDynamic(this,&AEnemy::OnPlayerAttackOverlapEnd);

	EnemyAIController->RandomPatrol();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!bPlayerDetected)
	{
		EnemyAIController->RandomPatrol();
	}
	else if (bPlayerDetected && bCanAttackPlayer)
	{
		StopSeekingPlayer();

		// attack player
		Attack();
		UE_LOG(LogTemp, Warning, TEXT("Player ATTACKED"));
	}
}


void AEnemy::MoveToPlayer()
{
	if(Player)
	{
		EnemyAIController->MoveToLocation(Player->GetActorLocation(), StoppingDistance, true);
	}
}

void AEnemy::SeekPlayer()
{
	MoveToPlayer();
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this, &AEnemy::SeekPlayer, 0.25f, true);
}

void AEnemy::StopSeekingPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

void AEnemy::DamageEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("Damaging Enemy"));

	Health -= 20.0f;

	if (Health <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Dead"));
		bDead = true;
	}
}

void AEnemy::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack - PARENT CLASS"));
}


void AEnemy::OnPlayerDetectedOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<ATheGreatEscapeCharacter>(OtherActor);
	
	if (Player)
	{
		bPlayerDetected = true;
		SeekPlayer();
	}
}

void AEnemy::OnPlayerDetectedOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<ATheGreatEscapeCharacter>(OtherActor);
	
	if (Player)
	{
		bPlayerDetected = false;
		StopSeekingPlayer();
		EnemyAIController->RandomPatrol();
	}
}

void AEnemy::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<ATheGreatEscapeCharacter>(OtherActor);
	
	if (Player)
	{
		bCanAttackPlayer = true;
	}
}

void AEnemy::OnPlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<ATheGreatEscapeCharacter>(OtherActor);
	
	if (Player)
	{
		bCanAttackPlayer = false;
		SeekPlayer();
	}
}


