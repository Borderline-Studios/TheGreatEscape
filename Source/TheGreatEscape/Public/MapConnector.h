// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : MapConnector.h
// Description : Contains the declarations and definitions for the MapConnector c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MapConnector.generated.h"

class ASplineTrack;

UCLASS()
class THEGREATESCAPE_API AMapConnector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapConnector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when the game ends or lifetime of the instance is coming to an end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void LevelLoadSequence();

private:
	// FUNCTIONS
	UFUNCTION()
	void BeginFrontOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult
	);


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif

	// VARIABLES
	// Player Detection Component
	UBoxComponent* CollisionDetection;

	// Spline Reference
	UPROPERTY(EditInstanceOnly)
	ASplineTrack* SplineRef;

	// Next World Reference
	UPROPERTY(EditInstanceOnly)
	TSoftObjectPtr<UWorld> NextWorldRef;
	FTimerHandle LoadLevelHandle;

	// Player Reference
	AActor* PlayerRef;
	
	// Camera Variables
	UPROPERTY(EditInstanceOnly)
	float BlendTime = 8.0f;

	UCameraComponent* PlayerCamRef;
	
	// UPROPERTY(EditInstanceOnly)
	float ZHeight = 100.0f;
};
