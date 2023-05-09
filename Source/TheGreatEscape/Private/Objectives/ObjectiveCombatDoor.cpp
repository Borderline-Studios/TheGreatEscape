// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Objectives/ObjectiveCombatDoor.h"

#include "Character/Player/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObjectiveCombatDoor::AObjectiveCombatDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = Root;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction Indicator"));
	Arrow->SetupAttachment(RootComponent);

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame"));
	Frame->SetupAttachment(RootComponent);
	
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(RootComponent);
	DoorStartPosition = Door->GetComponentLocation();

	PlayerDetector = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Detector"));
	PlayerDetector->SetupAttachment(RootComponent);
	PlayerDetector->OnComponentBeginOverlap.AddDynamic(this, &AObjectiveCombatDoor::BeginDetectorOverlap);
	PlayerDetector->OnComponentEndOverlap.AddDynamic(this, &AObjectiveCombatDoor::EndDetectorOverlap);
}

// Called when the game starts or when spawned
void AObjectiveCombatDoor::BeginPlay()
{
	Super::BeginPlay();
	
	DoorStartPosition = Door->GetComponentLocation();

	ForwardDetectionPosition = GetActorForwardVector() * 250;

	if (!PlayerRef)
	{
		PlayerRef = UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass());
	}

	if (MovementCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, TEXT("ProcessMovement"));
		MovementTimeline.AddInterpFloat(MovementCurve, ProgressFunction);
		
		MovementTimeline.SetTimelineLengthMode(TL_LastKeyFrame);
		const float FloatToComplete = DoorSFX->GetDuration();
		MovementTimeline.SetPlayRate(1.0f/FloatToComplete);
		MovementTimeline.SetLooping(false);
	}
}

void AObjectiveCombatDoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AObjectiveCombatDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimeline.IsPlaying())
	{
		MovementTimeline.TickTimeline(DeltaTime);
	}
}

void AObjectiveCombatDoor::BeginDetectorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == PlayerRef)
	{
		MovementTimeline.PlayFromStart();
	}
}

void AObjectiveCombatDoor::EndDetectorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor == PlayerRef)
	{
		MovementTimeline.ReverseFromEnd();
		// float DistanceFromFront = FVector::Dist(ForwardDetectionPosition, PlayerRef->GetActorLocation());
		// float DistanceFromBack = FVector::Dist(-ForwardDetectionPosition, PlayerRef->GetActorLocation());
		//
		// if (DistanceFromFront < DistanceFromBack && bDoorCanOpen)
		// {
		// 	LockDoor();
		// }
	}
}

void AObjectiveCombatDoor::ProcessTimeline(float TimelineProgress) const
{
	const FVector CurrentDoorPosition = FMath::Lerp(DoorStartPosition, DoorStartPosition + FVector(DoorMoveDistance, 0.0f, 0.0f), TimelineProgress);

	Door->SetRelativeLocation(CurrentDoorPosition);
}

void AObjectiveCombatDoor::LockDoor()
{
	bDoorCanOpen = false;
}

