// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkHybridTank.cpp
// Description : Hybrid Tank enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "EnemyReworkHybridTank.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

/**
 * @brief Constructor
 */
AEnemyReworkHybridTank::AEnemyReworkHybridTank()
{
  if (!LoadedBpProjectile)
  {
   // Find class we want
   ProjectileClass = TSoftClassPtr<AHybridEnemyProjectile>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Rework/BP_HybridProjectile.BP_HybridProjectile_C'")));

   // Load the class we want onto class pointer
   LoadedBpProjectile = ProjectileClass.LoadSynchronous();
  }
}


/**
 * @brief Sets up the tree using Drone enemy type
 * @param NewController Controller enemy is possessed by
 */
void AEnemyReworkHybridTank::PossessedBy(AController* NewController)
{
	Super::Super::Super::PossessedBy(NewController);
}

void AEnemyReworkHybridTank::BeginPlay()
{
 Super::BeginPlay();

 PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

/**
 * @brief Tick function
 * @param DeltaTime deltatime
 */
void AEnemyReworkHybridTank::Tick(float DeltaTime)
{
 Super::Tick(DeltaTime);

 if (PlayerRef)
 {
  FVector Forward = PlayerRef->GetActorLocation() - GetActorLocation();
  FRotator Rot = UKismetMathLibrary::MakeRotFromXY(Forward, UKismetMathLibrary::Cross_VectorVector(Forward, FVector::DownVector));
  
  // set rotation of the actor
  SetActorRotation(Rot);
 }
 
 // if drone can attack
 if (bCanAttack)
 {    
  // Spawn Parameters
  SpawnParams.Owner = this;
  SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
  // SET ANIM CALL
  GetMesh()->GetAnimInstance()->Montage_JumpToSection("Shoot");
  GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &AEnemyReworkHybridTank::ShootGun);
   
  bCanAttack = false;
  GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &AEnemyReworkHybridTank::SetCanAttack, AttackDelay, false);
    
 }
}

void AEnemyReworkHybridTank::ShootGun(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
 if (LoadedBpProjectile)
 {
		
  if (NotifyName == FName("LeftTurretShot"))
  {
    // SPawn projectile
    FVector LeftTurretLoc = GetMesh()->GetSocketLocation("L_WristSocket");
    //FRotator LeftTurretRot2 = FRotator(GetActorRotation().Pitch, GetMesh()->GetSocketRotation("L_WristSocket").Yaw, GetMesh()->GetSocketRotation("L_WristSocket").Roll);
    //FRotator LeftTurretRotSide1 = FRotator(GetActorRotation().Pitch, GetMesh()->GetSocketRotation("L_WristSocket").Yaw + 5.0f, GetMesh()->GetSocketRotation("L_WristSocket").Roll);

    float Angle = 15.0f;
    FRotator LeftTurretRot = FRotator(GetActorRotation().Pitch, GetMesh()->GetSocketRotation("L_WristSocket").Yaw - Angle, GetMesh()->GetSocketRotation("L_WristSocket").Roll);
   
    for (int i = 0; i < 10; i++)
    {
       GetWorld()->SpawnActor<AHybridEnemyProjectile>(LoadedBpProjectile, LeftTurretLoc, LeftTurretRot, SpawnParams);
       LeftTurretRot.Add(0.0f, Angle, 0.0f);
       UE_LOG(LogTemp, Warning, TEXT("left shoot"));
       
    }
   
    
    //GetWorld()->SpawnActor<AHybridEnemyProjectile>(LoadedBpProjectile, LeftTurretLoc, LeftTurretRotSide1, SpawnParams);
    //GetWorld()->SpawnActor<AHybridEnemyProjectile>(LoadedBpProjectile, LeftTurretLoc, LeftTurretRotSide2, SpawnParams);

  }
  if (NotifyName == FName("RightTurretShot"))
  {
    // SPawn projectile
    FVector RightTurretLoc = GetMesh()->GetSocketLocation("R_WristSocket");
    //FRotator RightTurretRotMid = FRotator(GetActorRotation().Pitch, GetMesh()->GetSocketRotation("R_WristSocket").Yaw, GetMesh()->GetSocketRotation("R_WristSocket").Roll);
    //FRotator RightTurretRotSide1 = FRotator(GetActorRotation().Pitch, GetMesh()->GetSocketRotation("R_WristSocket").Yaw + 15.0f, GetMesh()->GetSocketRotation("R_WristSocket").Roll);
    //FRotator RightTurretRotSide2 = FRotator(GetActorRotation().Pitch, GetMesh()->GetSocketRotation("R_WristSocket").Yaw - 15.0f, GetMesh()->GetSocketRotation("R_WristSocket").Roll);

   float Angle = 15.0f;
   FRotator RightTurretRot = FRotator(GetActorRotation().Pitch, GetMesh()->GetSocketRotation("R_WristSocket").Yaw - Angle, GetMesh()->GetSocketRotation("R_WristSocket").Roll);
   
   for (int i = 0; i < 3; i++)
   {
     GetWorld()->SpawnActor<AHybridEnemyProjectile>(LoadedBpProjectile, RightTurretLoc, RightTurretRot, SpawnParams);
     RightTurretRot.Add(0.0f, Angle, 0.0f);
     UE_LOG(LogTemp, Warning, TEXT("right shoot"));
       
   }
  }
 }
}

void AEnemyReworkHybridTank::SetCanAttack()
{
 // can attack
 bCanAttack = true;

 // clear timer
 GetWorld()->GetTimerManager().ClearTimer(AttackDelayHandle);
}


void AEnemyReworkHybridTank::PostDeathProcess()
{
 UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSFX, this->GetActorLocation(), this->GetActorRotation(), 0.5, FMath::RandRange(0.7, 1.4));
 // Spawn in body parts
 FVector location =  this->GetActorLocation();
 AActor* deathEffect = Cast<AActor>(GetWorld()->SpawnActor(DeathEffectReferenceHyrbid, &location));
 UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, this->GetActorLocation(), this->GetActorRotation(), FVector(0.5,0.5,0.5), true);
}

void AEnemyReworkHybridTank::PostHitProcess()
{
 Super::PostHitProcess();
 UE_LOG(LogTemp, Warning, TEXT("Hybrid hit"));
}