// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : BTTask_PlayAudio.h
// Description : Plays audio when it sees the player
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PlayAudio.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_PlayAudio : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
 	// *** Functions *** ///
 	UBTTask_PlayAudio(FObjectInitializer const& ObjectInitializer);
 	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
 	
private:
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta = (AllowPrivateAccess = "true"))
    USoundBase* AlertSFX; // Alert sfx
    
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    //float WanderMoveSpeed = 400.0f; // Wander speed of the enemy
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
    float RunMoveSpeed = 500.0f; // Run speed of the enemy
};
