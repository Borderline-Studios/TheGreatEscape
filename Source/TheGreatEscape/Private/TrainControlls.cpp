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
	ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMesh(TEXT("StaticMesh'/Game/Production/Train/Art/Quantum_Train/Engine/Engine_Lever_Base.Engine_Lever_Base'"));
	ControlBase->SetStaticMesh(BaseMesh.Object);
	ControlBase->SetWorldScale3D(FVector(3.0f));

	ControlHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle Mesh"));
	ControlHandle->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> HandleMesh(TEXT("StaticMesh'/Game/Production/Train/Art/Quantum_Train/Engine/Engine_Lever_Handle.Engine_Lever_Handle'"));
	ControlHandle->SetStaticMesh(HandleMesh.Object);
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

	if (EngineRef)
	{
		UpdateHandleRotation(EngineRef->GetTrainMoving());
	}

	if (RotationCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, TEXT("ProcessRotation"));
		RotationTimeline.AddInterpFloat(RotationCurve, ProgressFunction);

		RotationTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
	}
}

// Called every frame
void ATrainControlls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RotationTimeline.IsPlaying())
	{
		RotationTimeline.TickTimeline(DeltaTime);
	}
}

void ATrainControlls::ControlsInteraction()
{
	if (EngineRef)
	{
		EngineRef->ToggleTrainStop(true, true);

		PlayLeverSound();
		UpdateHandleRotation(EngineRef->GetTrainMoving());
	}
}

void ATrainControlls::UpdateHandleRotation(bool bTrainMoving)
{
	if (bTrainMoving)
	{
		RotationTimeline.Reverse();
	}
	else
	{
		RotationTimeline.Play();
	}
}

void ATrainControlls::PlayLeverSound()
{
	if (!LeverSFX.IsEmpty())
	{
		const int RandIndex = FMath::RandRange(0,LeverSFX.Num() - 1);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LeverSFX[RandIndex], GetActorLocation(), FRotator(), 0.6f);
	}
}

void ATrainControlls::ProcessRotation(float TimelineProgress) const
{
	ControlHandle->SetRelativeRotation(FMath::Lerp(ForwardRotation, BackwardRotation, TimelineProgress));
}

