// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : GateSphereComponent.cpp
// Description : Contains the implementation of the GateSphereComponent c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "Objectives/GateSphereComponent.h"
#include "Components/SplineComponent.h"
#include "SplineTrack.h"

#if WITH_EDITOR
/**
 * @brief 
 * @param bFinished 
 */
void UGateSphereComponent::PostEditComponentMove(bool bFinished)
{
	Super::PostEditComponentMove(bFinished);

	if (SplineRef)
	{
		SetWorldLocation(SplineRef->FindLocationClosestToWorldLocation(GetComponentLocation(), ESplineCoordinateSpace::World));
	}
}

void UGateSphereComponent::PostEditUndo()
{
	Super::PostEditUndo();

	if (SplineRef)
	{
		SetWorldLocation(SplineRef->FindLocationClosestToWorldLocation(GetComponentLocation(), ESplineCoordinateSpace::World));
	}
}
#endif

/**
 * @brief 
 * @param NewSplineRef 
 */
void UGateSphereComponent::InitialiseFromGate(const ASplineTrack* NewSplineRef)
{
	if (!SplineRef)
	{
		SplineRef = NewSplineRef->GetSpline();
		SetWorldLocation(SplineRef->FindLocationClosestToWorldLocation(GetComponentLocation(), ESplineCoordinateSpace::World));
	}
}
