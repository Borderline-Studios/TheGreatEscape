// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "SplineTraversalComponent.generated.h"

class USplineComponent;
class ASplineTrack;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class THEGREATESCAPE_API USplineTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplineTraversalComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// FUNCTIONS
	// Timeline
	UFUNCTION()
	void ProcessMovement(float TimelineProgress) const;
	UFUNCTION()
	void OnEndMovementTimeline();

	// VARIABLES
	// Timeline
	FTimeline MovementTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "Editable")
	UCurveFloat* MovementCurve;

	// Spline
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true", Category = "Editable"))
	ASplineTrack* SplineParent;
	UPROPERTY()
	USplineComponent* SplineRef = nullptr;
	int SplineLength = 0;

	// Parent Details
	UPROPERTY()
	AActor* OwnerRef = nullptr;

	// Spline Traversal Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess), Category = "Editable")
	int TimeToComplete = 30;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess), Category = "Editable")
	bool bLoop = false;

public:
	// FUNCTIONS


	// VARIABLES
	
};
