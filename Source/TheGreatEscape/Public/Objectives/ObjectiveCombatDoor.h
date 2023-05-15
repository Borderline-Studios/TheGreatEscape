// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : ObjectiveCombatDoor.h
// Description : Contains the definitions and declarations for the ObjectiveCombatDoor c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ObjectiveCombatDoor.generated.h"

// Forward Declared Classes
class UBoxComponent;
class UArrowComponent;

UCLASS()
class THEGREATESCAPE_API AObjectiveCombatDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveCombatDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called when the game ends or lifetime of the instance is coming to an end
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// FUNCTIONS
	UFUNCTION()
	void BeginDetectorOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);
	UFUNCTION()
	void EndDetectorOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	UFUNCTION()
	void ProcessTimeline(float TimelineProgress) const;

	void LockDoor();

	// VARIABLES
	USceneComponent* Root;

	// Mesh Variables
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Frame = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Door = nullptr;

	// Door Locking bool
	bool bDoorCanOpen = true;

	// Reference to Player
	inline static AActor* PlayerRef;
	FVector ForwardDetectionPosition;
	// Collision Detection Component
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* PlayerDetector;

	// bool to track what direction the door opens
	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess = "true"))
	bool bOpenLeft = true;

	// Door movement tracking
	FVector DoorStartPosition;
	const float DoorMoveDistance = 300;

	// Door sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX",  meta = (AllowPrivateAccess = "true"))
	USoundBase* DoorSFX;

	// Direction Indicator, the door is supposed to be one way
	UArrowComponent* Arrow;

	// Timeline Setup
	FTimeline MovementTimeline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "Editable")
	UCurveFloat* MovementCurve;

public:
	// FUNCTIONS
	void ReleaseDoor();

	// VARIABLES
};
