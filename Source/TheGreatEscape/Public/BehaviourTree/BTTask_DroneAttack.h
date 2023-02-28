// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DroneAttack.generated.h"

/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UBTTask_DroneAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_DroneAttack(FObjectInitializer const& ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


private:
	UFUNCTION()
	void SetCanAttack();

	FTimerHandle AttackDelayHandle;
	bool bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackDelay = 3.0f;
	
};
