// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainTrackPoint.h"

#include "TrainTrack.h"
#include "Kismet/GameplayStatics.h"

// Static Variable declaration
TArray<ATrainTrackPoint*> ATrainTrackPoint::StaticArray;

// Sets default values
ATrainTrackPoint::ATrainTrackPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Marker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Position Marker"));
	RootComponent = Marker;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	Marker->SetStaticMesh(MeshObj.Object);

	AActor* ActorCollector = UGameplayStatics::GetActorOfClass(this, ATrainTrack::StaticClass());
    if (ATrainTrack* TempRef = Cast<ATrainTrack>(ActorCollector))
    {
	    TrackRef = TempRef;
    }
}

ATrainTrackPoint::~ATrainTrackPoint()
{
	if (TrackRef)
	{
		TrackRef->RemoveFromArray(this);
	}
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

void ATrainTrackPoint::AddToStaticArray()
{
	if (TrackRef)
	{
		TrackRef->AddToArray(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TrackRef hasn't been populated, destroying this item"));
	}
	// if (StaticArray.Find(this) == INDEX_NONE /*&& !IsInPersistentLevel()*/)
	// {
	// 	StaticArray.AddUnique(this);
	//
	// 	UE_LOG(LogTemp, Warning, TEXT("The Current size of the Static Array is %i"), StaticArray.Num());
	// 	UE_LOG(LogTemp, Warning, TEXT("The world this item exists in is %s"), *GetWorld()->GetMapName());
	//
	// 	for (int i = 0; i < StaticArray.Num(); i++)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Point #%i exists in world %s"), i, *StaticArray[i]->GetWorld()->GetMapName());
	// 	}
	// }
}

TArray<ATrainTrackPoint*> ATrainTrackPoint::GetStaticArray()
{
	return StaticArray;
}

#if WITH_EDITOR
void ATrainTrackPoint::PostActorCreated()
{
	// AddToStaticArray();
}
#endif