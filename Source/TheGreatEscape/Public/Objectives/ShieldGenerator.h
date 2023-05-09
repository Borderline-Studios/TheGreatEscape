// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShieldGenerator.generated.h"

UCLASS()
class THEGREATESCAPE_API AShieldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShieldGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageToDo = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 20;

	UFUNCTION(BlueprintImplementableEvent)
	void PostHitProcess();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
