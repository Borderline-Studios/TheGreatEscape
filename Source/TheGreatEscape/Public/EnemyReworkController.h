// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : EnemyReworkController.h
// Description : Custom ai controller
// Author      :  Borderline Studios - Toni Natta
// Mail        :  toni.natta@mds.ac.nz

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviourTree/BlackboardKeys.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyReworkController.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API AEnemyReworkController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyReworkController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());
	void BeginPlay() override;
	void OnPossess(APawn* const InPawn) override;
	class UBlackboardComponent* GetBlackboard() const;

	void SetBehaviourTree(Utilities::EnemyTypes EEnemyType);
	
	//Utilities::EnemyTypes EEnemyType = Utilities::EnemyTypes::Melee;

	

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTreeMelee;
	class UBehaviorTree* BehaviorTreeDrone;
	class UBehaviorTree* BehaviorTree;


private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	class UBlackboardComponent* Blackboard;

	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);
	void SetupPerceptionSystem();
	
};
