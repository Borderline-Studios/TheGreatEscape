// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainTrack.h"

// Sets default values
ATrainTrack::ATrainTrack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	Spline->SetupAttachment(RootComponent);

	StartNotifier = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Start"));
	StartNotifier->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	StartNotifier->SetStaticMesh(MeshObj.Object);
	StartNotifier->SetWorldScale3D(FVector(0.25f, 1.0f, 1.0f));
	ConstructorHelpers::FObjectFinder<UMaterial> StartMat(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Copper.M_Metal_Copper'"));
	StartNotifier->SetMaterial(0, StartMat.Object);
	
	EndNotifier = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("End"));
	EndNotifier->SetupAttachment(RootComponent);
	EndNotifier->SetStaticMesh(MeshObj.Object);
	EndNotifier->SetWorldScale3D(FVector(0.25f, 1.0f, 1.0f));
	ConstructorHelpers::FObjectFinder<UMaterial> EndMat(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'"));
	EndNotifier->SetMaterial(0, EndMat.Object);

	StartNotifier->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::Local));
	EndNotifier->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::Local));

	Spline->SetUnselectedSplineSegmentColor(FLinearColor(FColor::Purple));
	Spline->SetSelectedSplineSegmentColor(FLinearColor(FColor::Green));
	Spline->SetTangentColor(FLinearColor(FColor::Red));
}

// Called when the game starts or when spawned
void ATrainTrack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrainTrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR
void ATrainTrack::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	// Code that runs during the move action as well as one more time at the end of that action:
	StartNotifier->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	StartNotifier->SetWorldRotation(Spline->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	EndNotifier->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
	EndNotifier->SetWorldRotation(Spline->GetRotationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));

	// Limit the tangents
	// Code only runs here during specific stages of the Editor Move
	if (!bFinished)	// Only runs during the actual moving of the item
	{
	
	}
	else			// Only runs at the end of the Move action
	{
	
	}
}

void ATrainTrack::PostEditUndo()
{
	Super::PostEditUndo();

	StartNotifier->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	StartNotifier->SetWorldRotation(Spline->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World));
	EndNotifier->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
	EndNotifier->SetWorldRotation(Spline->GetRotationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
}
#endif