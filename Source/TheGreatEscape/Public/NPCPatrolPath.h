// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : NPCPatrolPath.h
// Description : Keeps track of the patrol path for the enemy
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCPatrolPath.generated.h"

UCLASS()
class THEGREATESCAPE_API ANPCPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCPatrolPath();

	FVector GetPatrolPoint(int const index) const;
	int Num() const;

private:
	// Allow to be edited in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = true, AllowPrivateAccess = true))
	TArray<FVector> PatrolPoints;

};
