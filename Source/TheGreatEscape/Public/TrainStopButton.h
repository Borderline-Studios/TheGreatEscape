// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainStopButton.generated.h"

class ATrainEngine;
UCLASS()
class THEGREATESCAPE_API ATrainStopButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainStopButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//** Custom Functionality **//
	// VARIABLES


	// FUNCTIONS
	void ToggleTrainMovement();

protected:


private:
	// VARIABLES
	UStaticMeshComponent* ButtonMesh;
	ATrainEngine* EngineRef;

	// FUNCTIONS

};
