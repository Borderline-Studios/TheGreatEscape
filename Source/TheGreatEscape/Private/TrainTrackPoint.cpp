// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainTrackPoint.h"

// Static Variable declaration
TArray<ATrainTrackPoint*> ATrainTrackPoint::StaticArray;

// Sets default values
ATrainTrackPoint::ATrainTrackPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Marker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Position Marker"));
	Marker->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Marker->SetStaticMesh(MeshObj.Object);
	
	StaticArray.AddUnique(this);
}

ATrainTrackPoint::~ATrainTrackPoint()
{
	StaticArray.RemoveAt(StaticArray.Find(this));
}

// Called when the game starts or when spawned
void ATrainTrackPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrainTrackPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<ATrainTrackPoint*> ATrainTrackPoint::GetStaticArray()
{
	return StaticArray;
}

