// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_HybridAttack.cpp
// Description : Attack functionality for the hybrid
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "BehaviourTree/BTTask_HybridAttack.h"

#include "EnemyReworkController.h"
#include "EnemyReworkHybrid.h"
#include "Engine/SkeletalMeshSocket.h"

/**
 * @brief constructor, name the node
 * @param ObjectInitializer Finalise creation after c++ constructor is called 
 */
UBTTask_HybridAttack::UBTTask_HybridAttack(FObjectInitializer const& ObjectInitializer)
{
  NodeName = TEXT("Hybrid Attack");

  if (!LoadedBpProjectile)
  {
   // Find class we want
   ProjectileClass = TSoftClassPtr<AHybridEnemyProjectile>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Rework/BP_HybridProjectile.BP_HybridProjectile_C'")));

   // Load the class we want onto class pointer
   LoadedBpProjectile = ProjectileClass.LoadSynchronous();
  }
}

/**
 * @brief When node is executed it attacks the train or player
 * @param OwnerComp The owning behaviour tree component
 * @param NodeMemory Node's memory
 * @return result of the node (successful or not)
 */
EBTNodeResult::Type UBTTask_HybridAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
 // get Ai controller & check AI controller is not nullptr
 if (const AEnemyReworkController* AIController = Cast<AEnemyReworkController>(OwnerComp.GetAIOwner()))
 {
   // Get enemy
   AEnemyReworkHybrid* const Enemy = Cast<AEnemyReworkHybrid>(AIController->GetPawn());
   // if drone can attack
   if (bCanAttack)
   {
    // attack the train, set can attack to false and start timer
    //UE_LOG(LogTemp, Warning, TEXT("Hybrid attack"));

    // set attacking bool for animation
    Enemy->bAttacking = true;

    // Spawn Parameters
    SpawnParams.Owner = Enemy;

    LeftTurret = Enemy->LeftTurretRef;
    RightTurret = Enemy->RightTurretRef;
    
    // SET ANIM CALL
     Enemy->GetMesh()->GetAnimInstance()->Montage_JumpToSection("Shoot");
   
     Enemy->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UBTTask_HybridAttack::ShootGun);
    
   
     bCanAttack = false;
     Enemy->bAttacking = false;
    
     GetWorld()->GetTimerManager().SetTimer(AttackDelayHandle, this, &UBTTask_HybridAttack::SetCanAttack, AttackDelay, false);
   }
  // Finish task
  FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
  return EBTNodeResult::Succeeded;
 }
	
 // Log warning that cast failed and finish task
 UE_LOG(LogTemp, Warning, TEXT("Cast to AEnemyReworkController failed, task failed"));
 FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
 return EBTNodeResult::Failed;
 
}

/**
 * @brief Sets the attack bool to true and clears the timer
 */
void UBTTask_HybridAttack::SetCanAttack()
{
 // can attack
 bCanAttack = true;

 // clear timer
 GetWorld()->GetTimerManager().ClearTimer(AttackDelayHandle);
}

void UBTTask_HybridAttack::ShootGun(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
 
 if (LoadedBpProjectile)
 {
		
		
  if (NotifyName == FName("LeftTurretShot"))
  {
   // SPawn projectile
   AHybridEnemyProjectile* Projectile = GetWorld()->SpawnActor<AHybridEnemyProjectile>(LoadedBpProjectile, LeftTurret.GetLocation(), LeftTurret.GetRotation().Rotator(), SpawnParams);

   if (!Projectile)
   {
    UE_LOG(LogTemp, Warning, TEXT("Projectile class not found LEFT"));
   }
   else
   {
    UE_LOG(LogTemp, Warning, TEXT("SPAWNED PROJ"))
   }
  }
  if (NotifyName == FName("RightTurretShot"))
  {
   // SPawn projectile
   AHybridEnemyProjectile* Projectile = GetWorld()->SpawnActor<AHybridEnemyProjectile>(LoadedBpProjectile, RightTurret.GetLocation(), RightTurret.GetRotation().Rotator(), SpawnParams);

   if (!Projectile)
   {
    UE_LOG(LogTemp, Warning, TEXT("Projectile class not found RIGHT"));
   }
   else
   {
    UE_LOG(LogTemp, Warning, TEXT("SPAWNED PROJ"))
   }
  }
			
		
 }
}
