// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   :
// Description :
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveParent.generated.h"

class UBoxComponent;
class USplineComponent;
UCLASS()
class THEGREATESCAPE_API AObjectiveParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// VARIABLES


	// FUNCTIONS
	void PopulateSplineRef(USplineComponent* NewRef);

protected:
	// VARIABLES
	static USplineComponent* SplineReference;

	// FUNCTIONS
	
private:
	// VARIABLES
	UBoxComponent* Detector;

	// FUNCTIONS
	
};
