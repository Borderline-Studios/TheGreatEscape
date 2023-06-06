// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveShield.generated.h"

UCLASS()
class THEGREATESCAPE_API AObjectiveShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DamageToBreak = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int ShieldHealth = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ResetTime = 30;


	void PostHitProcess();
	void PostDestroyProcess();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateVFX(FHitResult InputHit); 

};
