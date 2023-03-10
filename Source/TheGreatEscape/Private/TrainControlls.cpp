// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : TrainControls.h
// Description : Updates orientation of train controls based on the train setting
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz


#include "TrainControlls.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrainControlls::ATrainControlls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!EngineRef)
	{
		EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
	}

	ControlBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = ControlBase;
	ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("StaticMesh'/Game/Production/Train/Art/TrackSwitch/Lever_Lever_base.Lever_Lever_base'"));
	ControlBase->SetStaticMesh(BaseMesh.Object);
	ControlBase->SetWorldScale3D(FVector(3.0f));

	ControlHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle Mesh"));
	ControlHandle->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> HandleMesh(TEXT("StaticMesh'/Game/Production/Train/Art/TrackSwitch/Lever_Lever.Lever_Lever'"));
	ControlHandle->SetStaticMesh(HandleMesh.Object);
	ControlHandle->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	Tags.Push("Interactable");
}

// Called when the game starts or when spawned
void ATrainControlls::BeginPlay()
{
	Super::BeginPlay();

	if (!EngineRef)
	{
		EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
	}
}

// Called every frame
void ATrainControlls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrainControlls::UpdateEngineSpeed()
{
	switch (ControlSetting)
	{
	default:
	case ETrainSpeed::Slow:
		ControlSetting = ETrainSpeed::Standard;
		break;
	case ETrainSpeed::Standard:
		ControlSetting = ETrainSpeed::Fast;
		break;
	case ETrainSpeed::Fast:
		ControlSetting = ETrainSpeed::Slow;
		break;
	}

	EngineRef->SetTrainSpeed(ControlSetting);

	UpdateControls();
}

/**
	 * @brief Update the train control rotation based on the setting
	 * @param ControlSetting The Setting you wish to change to
	 */
void ATrainControlls::UpdateControls()
{
	CurrentRotation = ControlHandle->GetRelativeRotation();
	// float RotateValue;
	
	// Rotate the mesh based on the setting
	switch (ControlSetting)
	{
	case ETrainSpeed::Slow:
		{
			ControlHandle->SetRelativeRotation(FRotator(0.0f, 0.0f, -45.0f));
			
			// RotateValue = -50.0f;
			// ControlHandle->SetRelativeRotation(FMath::Lerp(FQuat(CurrentRotation), FQuat(FRotator(RotateValue, 0.0f, 0.0f)), 0.01f));
		}
		break;
	case ETrainSpeed::Standard:
		{
			ControlHandle->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

			// RotateValue = 0.0f;
			// ControlHandle->SetRelativeRotation(FMath::Lerp(FQuat(CurrentRotation), FQuat(FRotator(RotateValue, 0.0f, 0.0f)), 0.01f));
		}
		break;
	case ETrainSpeed::Fast:
		{
			ControlHandle->SetRelativeRotation(FRotator(0.0f, 0.0f, 45.0f));
			
			// RotateValue = 50.0f;
			// ControlHandle->SetRelativeRotation(FMath::Lerp(FQuat(CurrentRotation), FQuat(FRotator(RotateValue, 0.0f, 0.0f)), 0.01f));
		}
		break;
	default: ;
	}
}

