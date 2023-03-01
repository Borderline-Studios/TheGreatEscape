// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : NPC.h
// Description : Npc character class
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "Character/BASE/GASBASECharacter.h"
#include "NPCPatrolPath.h"
#include "NPC.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API ANPC : public AGASBASECharacter
{
	GENERATED_BODY()

public:
	ANPC();
	ANPCPatrolPath* GetPatrolPath();
	virtual void PossessedBy(AController* NewController) override;

private:
	// NPC STUFF
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	ANPCPatrolPath* PatrolPath;
	
};
