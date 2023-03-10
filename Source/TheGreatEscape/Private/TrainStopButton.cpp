// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "TrainStopButton.h"

#include "TrainEngine.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrainStopButton::ATrainStopButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Mesh"));
	RootComponent = ButtonMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	ButtonMesh->SetStaticMesh(MeshObj.Object);
	ButtonMesh->SetWorldScale3D(FVector(0.25f, 0.15f, 0.15f));

	EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
	Tags.Push("Interactable");
}

// Called when the game starts or when spawned
void ATrainStopButton::BeginPlay()
{
	Super::BeginPlay();

	if (!EngineRef)
	{
		EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
	}
}

// Called every frame
void ATrainStopButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrainStopButton::ToggleTrainMovement()
{
	if (EngineRef)
	{
		EngineRef->ToggleTrainStop();
	}
}

