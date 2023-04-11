// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Character/Player/PlayerCharacter.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"

#include "TrainCarParent.generated.h"

UCLASS(Abstract)
class THEGREATESCAPE_API ATrainCarParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainCarParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// FUNCTIONS
	inline static void ClearStaticTimers();
	bool CheckTrainForPlayer() const;

	// VARIABLES
	inline static bool bClearedStaticTimers = false;
	inline static bool bPlayerOnTrain = false;
	inline static TArray<UBoxComponent*> PlayerDetectionBoxes;
	inline static FTimerHandle PlayerDetectionTimerHandle;
	inline static UWorld* CurrentWorldRef = nullptr;

protected:
	// FUNCTIONS
	UFUNCTION()
	virtual void BeginCarOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);
	UFUNCTION()
	virtual void EndCarOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	void SetCurrentWorld(UWorld* NewWorldRef);
	bool GetPlayerOnTrain();
	static void PushPlayerDetectionComponent(UBoxComponent* NewCollisionRef);
	
	void EnableTrainMovementTimer() const;
	void DisableTrainMovementTimer() const;

	// VARIABLES
	// Shared / Static variables
	inline static APlayerCharacter* PlayerRef = nullptr;
	inline static USplineComponent* SplineRef = nullptr;

	// Individual Variables
	USceneComponent* SceneRoot = nullptr;
	UStaticMeshComponent* CarMesh = nullptr;
	UBoxComponent* PlayerDetectionComponent = nullptr;
};
