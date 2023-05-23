// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : GateSphereComponent.h
// Description : Contains the declarations and definitions for the GateSphereComponent c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz
#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GateSphereComponent.generated.h"

// Forward Declared Classes
class USplineComponent;
class ASplineTrack;
/**
 * 
 */
UCLASS()
class THEGREATESCAPE_API UGateSphereComponent : public USphereComponent
{
	GENERATED_BODY()

private:
	// FUNCTIONS
// This means that these functions only exist and apply while in the editor.
// These do not apply to packages
#if WITH_EDITOR
	virtual void PostEditComponentMove(bool bFinished) override;
	virtual void PostEditUndo() override;
#endif

	// VARIABLES
	UPROPERTY()
	USplineComponent* SplineRef;

public:
	// FUNCTIONS
	void InitialiseFromGate(const ASplineTrack* NewSplineRef);

	// VARIABLES
	
};
