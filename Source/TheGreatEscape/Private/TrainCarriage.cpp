// Fill out your copyright notice in the Description page of Project Settings.


#include "TrainCarriage.h"

#include "Components/SplineComponent.h"

// Sets default values
ATrainCarriage::ATrainCarriage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Carriage Mesh"));
	Box->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	Box->SetStaticMesh(MeshObj.Object);
	Box->SetWorldScale3D(FVector(2.5f, 1.0f, 0.7f));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetArrowColor(FColor::Blue);
	Arrow->SetHiddenInGame(false);
	Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
}

ATrainCarriage::ATrainCarriage(int AssignedNumber)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Carriage Mesh"));
	Box->SetupAttachment(RootComponent);
	
	// Box->SetWorldScale3D(FVector(2.5f, 1.0f, 0.7f));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetArrowColor(FColor::Blue);
	Arrow->SetHiddenInGame(false);
	Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
}

// Called when the game starts or when spawned
void ATrainCarriage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrainCarriage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrainCarriage::SetCarriageNumber(int AssignedNumber)
{
	CarriageNumber = AssignedNumber;

	if 	(CarriageNumber == 0)	// Full Base Car
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> FullBase(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Base_Full_Train_Car_Base_Full.Train_Car_Base_Full_Train_Car_Base_Full'"));
		Box->SetStaticMesh(FullBase.Object);
	}
	else if (CarriageNumber == 1)	// FlatBed Car
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> FlatBed(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_FlatBed_Full_Train_Car_FlatBed_Full.Train_Car_FlatBed_Full_Train_Car_FlatBed_Full'"));
		Box->SetStaticMesh(FlatBed.Object);
	}
	else if (CarriageNumber == 2)	// Weapons Car
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Weapons(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Weapons_Full_Train_Car_Weapon_Full.Train_Car_Weapons_Full_Train_Car_Weapon_Full'"));
		Box->SetStaticMesh(Weapons.Object);
	}
	else if (CarriageNumber == 3)	// Windows Car 
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> Windows(TEXT("StaticMesh'/Game/Production/Train/Art/All_Train_Cars_Full/Train_Car_Windows_Full.Train_Car_Windows_Full'"));
		Box->SetStaticMesh(Windows.Object);
	}
}

bool ATrainCarriage::ChangeTrack(AActor* NewTrack)
{
	if (USplineComponent* TempSplineRef = Cast<USplineComponent>(NewTrack->GetRootComponent()); TempSplineRef == nullptr)
	{
		return false;
	}
	else
	{
		TrackSplineRef = TempSplineRef;
	}

	//TimeSinceStart = 0;
	//SplineLength = TrackSplineRef->GetSplineLength();

	return true;
}

