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
	UE_LOG(LogTemp, Warning, TEXT("Attack - CHILD Ranged class"));
	
	GetWorld()->SpawnActor<ATheGreatEscapeProjectile>(Projectile, GetActorLocation(), GetActorRotation());

	UE_LOG(LogTemp, Warning, TEXT("Location: %s"), *GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *GetActorRotation().ToString());
}
