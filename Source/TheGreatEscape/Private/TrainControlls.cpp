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

#include "TrainEngine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATrainControlls::ATrainControlls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrainControlls::BeginPlay()
{
	Super::BeginPlay();

	if (!EngineRef)
	{
		EngineRef = Cast<ATrainEngine>(UGameplayStatics::GetActorOfClass(this, ATrainEngine::StaticClass()));
	}

	// ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	
}

// Called every frame
void ATrainControlls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
	 * @brief Update the train control rotation based on the setting
	 * @param ControlSetting The Setting you wish to change to
	 */
void ATrainControlls::UpdateControls()
{
	CurrentRotation = TrainControlMesh->GetRelativeRotation();

	if (EngineRef)
	{
		EngineRef->SetTrainSpeed(ControlSetting);
	}

	// Rotate the mesh based on the setting
	switch (ControlSetting)
	{
	case ETrainControlSetting::Slow:
		{
			ControlSetting = ETrainControlSetting::Normal;
			RotateValue = -50.0f;
			TrainControlMesh->SetRelativeRotation(FMath::Lerp(FQuat(CurrentRotation), FQuat(FRotator(RotateValue, 0.0f, 0.0f)), 0.01f));
		}
	case ETrainControlSetting::Normal:
		{
			ControlSetting = ETrainControlSetting::Fast;
			RotateValue = 0.0f;
			TrainControlMesh->SetRelativeRotation(FMath::Lerp(FQuat(CurrentRotation), FQuat(FRotator(RotateValue, 0.0f, 0.0f)), 0.01f));
		}
	case ETrainControlSetting::Fast:
		{
			ControlSetting = ETrainControlSetting::Slow;
			RotateValue = 50.0f;
			TrainControlMesh->SetRelativeRotation(FMath::Lerp(FQuat(CurrentRotation), FQuat(FRotator(RotateValue, 0.0f, 0.0f)), 0.01f));
		}
	}
	
}
