// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveManager.h
// Description : Contains the declarations and definitions for the ObjectiveManager c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveManager.generated.h"

class ASplineTrack;
class USplineComponent;
class AObjectiveParent;
UCLASS()
class THEGREATESCAPE_API AObjectiveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// FUNCTIONS
	// Editor callable functions that interact with objectives
	UFUNCTION(CallInEditor)
	void AddPickupObjective();
	
	UFUNCTION(CallInEditor)
	void AddDefenseObjective();
	
	UFUNCTION(CallInEditor)
	void AddSecureObjective();
	
	UFUNCTION(CallInEditor)
	void UndoObjective();
	
	UFUNCTION(CallInEditor)
	void ClearObjectives();

	// Non-Editor Callable functions
	void UpdateArray();
	
	bool ReachedCap() const
	{
		return ObjectiveRefs.Num() > 4;
	}

#if WITH_EDITOR
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;
	// virtual void PostEditUndo() override;
#endif
	
	// VARIABLES
	UStaticMeshComponent* StaticMesh;

	// References
	UPROPERTY(EditInstanceOnly)
	ASplineTrack* SplineRef;

	UPROPERTY(VisibleInstanceOnly)
	TArray<AObjectiveParent*> ObjectiveRefs;
};
