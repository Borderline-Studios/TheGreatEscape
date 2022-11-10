// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyRanged.cpp
// Description : 
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "EnemyRanged.h"

#include "TheGreatEscape/TheGreatEscapeProjectile.h"

void AEnemyRanged::Attack()
{
	Super::Attack();
	
	UE_LOG(LogTemp, Warning, TEXT("Attacking = %s"), bAttacking ? TEXT("true") : TEXT("false") );
	
	
	GetWorld()->SpawnActor<ATheGreatEscapeProjectile>(Projectile, GetMesh()->GetSocketLocation("FiringLocation"), GetActorRotation());

	//UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *GetActorRotation().ToString());

	bAttacking = false;

	UE_LOG(LogTemp, Warning, TEXT("Attacking = %s"), bAttacking ? TEXT("true") : TEXT("false") );
}
