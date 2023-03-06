// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Tracks/TrainTrackStart.h"

// TArray<ATrainTrackMiddle*> ATrainTrackStart::MiddleRefs;

ATrainTrackStart::ATrainTrackStart()
{
	Spline->SetUnselectedSplineSegmentColor(FLinearColor(FColor::Green));
	Spline->SetSelectedSplineSegmentColor(FLinearColor(FColor::Purple));
	Spline->SetTangentColor(FLinearColor(FColor::Cyan));

	// StartMarker - Static Mesh noting the start location and rotation
	StartMarker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	StartMarker->SetStaticMesh(MeshObj.Object);
	ConstructorHelpers::FObjectFinder<UMaterial> MatObj(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
	StartMarker->SetMaterial(0, MatObj.Object);
	StartMarker->SetWorldScale3D(FVector(0.5f));

	StartMarker->SetHiddenInGame(true);
}

void ATrainTrackStart::BeginPlay()
{
	Super::BeginPlay();

	StartMarker->SetHiddenInGame(true);
}

void ATrainTrackStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrainTrackStart::BeginDestroy()
{
	Super::BeginDestroy();

	StartRef = nullptr;

	MarkedForDeath = true;
}

#if WITH_EDITOR
void ATrainTrackStart::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	StartMarker->SetWorldLocation(Spline->GetComponentLocation());
	StartMarker->SetWorldRotation(Spline->GetComponentRotation());
}
#endif

void ATrainTrackStart::AddToArray(ATrainTrackMiddle* NewTrackRef)
{
	MiddleRefs.Shrink();
	MiddleRefs.AddUnique(NewTrackRef);
}

void ATrainTrackStart::RemoveFromArray(ATrainTrackMiddle* NewTrackRef)
{
	// if (MiddleRefs.Find(NewTrackRef) != INDEX_NONE && !MarkedForDeath)
	// {
	// 	MiddleRefs.RemoveAt(MiddleRefs.Find(NewTrackRef));
	//
	// 	for (int i = 0; i < MiddleRefs.Num(); i++)
	// 	{
	// 		if (!(MiddleRefs[i]))
	// 		{
	// 			MiddleRefs.RemoveAt(i);
	// 		}
	// 	}
	//
	// 	MiddleRefs.Shrink();
	// }
}
