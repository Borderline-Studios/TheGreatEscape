// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ObjectiveCombatDoor.generated.h"

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

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Frame = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Door = nullptr;

	bool bDoorCanOpen = true;
	
	inline static AActor* PlayerRef;
	FVector ForwardDetectionPosition;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* PlayerDetector;

	float TimeSinceEnabled = 0;

	UPROPERTY(EditInstanceOnly, meta=(AllowPrivateAccess = "true"))
	bool bOpenLeft = true;
	
	FVector DoorStartPosition;
	const float DoorMoveDistance = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX",  meta = (AllowPrivateAccess = "true"))
	USoundBase* DoorSFX;

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
