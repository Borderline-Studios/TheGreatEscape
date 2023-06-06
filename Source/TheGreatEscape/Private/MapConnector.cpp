// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   : MapConnector.cpp
// Description : Contains the implementation of the MapConnector c++ class.
// Author      : Borderline Studios - Jake Laird
// Mail        : jake.laird@mds.ac.nz

#include "MapConnector.h"

//#include "Camera/CameraActor.h"
#include "SplineTrack.h"
#include "TrainEngine.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

/**
 * @brief Sets default values
 */
AMapConnector::AMapConnector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Populate and set up collision component, including a function delegate
	CollisionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionDetection"));
	CollisionDetection->SetupAttachment(RootComponent);
	CollisionDetection->SetWorldScale3D(FVector(25.0f));
	RootComponent = CollisionDetection;

	CollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &AMapConnector::BeginFrontOverlap);
}

/**
 * @brief Called when the game starts or when spawned
 */
void AMapConnector::BeginPlay()
{
	Super::BeginPlay();

	// Populate the player ref by getting the first actor of the PlayerCharacter class
	PlayerRef = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass());

	// Check if the player ref is populated
	if (!PlayerRef)
	{
		// Populate the player ref with the train engine
		PlayerRef = UGameplayStatics::GetActorOfClass(GetWorld(), ATrainEngine::StaticClass());

		// The default player character used multiple different classes
		// this check and assignment was a way to make sure the player could always change levels
	}
}

/**
 * @brief Called when the game ends or lifetime of the instance is coming to an end
 * @param EndPlayReason Reason for this function being called
 */
void AMapConnector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Clearing the timer manager prior to a new world being loaded
	GetWorldTimerManager().ClearTimer(LoadLevelHandle);
}

// Called every frame
void AMapConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

/**
* @brief OnComponentBeginOverlap delegate function,
 * used to detect if the player has entered the detection component
 * @param OverlappedComponent 
 * @param OtherActor 
 * @param OtherComp 
 * @param OtherBodyIndex 
 * @param bFromSweep 
 * @param SweepResult 
 */
void AMapConnector::BeginFrontOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	// Check if the other actor is the player
	if (OtherActor == PlayerRef)
	{
		// Insert other boolean checks here

		// Fade the camera out to black
		APlayerCameraManager* PlayerCamManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		PlayerCamManager->StartCameraFade(0.0, 1.0, BlendTime, FColor::Black, true, true);
		
		// Load the next level
		GetWorld()->GetTimerManager().SetTimer(LoadLevelHandle, [&]()
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, NextWorldRef);
		}, BlendTime + 1, false);


		LevelLoadSequence();
		// This all works as a rudimentary EndOfLevel Camera movement system but it's a bit involved for what we're after. Gonna alter it so that it fades instead
		// // Spawn the second camera
		// ACameraActor* PlayerCam = Cast<ACameraActor>(GetWorld()->SpawnActor(ACameraActor::StaticClass()));
		// PlayerCam->SetActorLocation(PlayerRef->GetActorLocation());
		// PlayerCam->GetCameraComponent()->SetConstraintAspectRatio(false);
		//
		// ACameraActor* TopCam = Cast<ACameraActor>(GetWorld()->SpawnActor(ACameraActor::StaticClass()));
		// TopCam->SetActorLocation(GetActorLocation() + FVector(0, 0, ZHeight));
		// // Set NewCam Rotation
		//
		// // Tween the camera using method one from that video
		// APlayerController* temp = UGameplayStatics::GetPlayerController(this, 0);
		// temp->SetViewTargetWithBlend(PlayerCam, 0);
		// temp->SetViewTargetWithBlend(TopCam, BlendTime);
		//
		// // Set for the camera to fade during the blend time, maybe through the use of another lambda TimerHandle
		//
	}
}

#if WITH_EDITOR// This means that these functions only exist and apply while in the editor. These do not apply to packages
/**
 * @brief Unreal Editor Function, called whenever a property is changed on a given actor in the editor window.
 * This snaps an instance of this class to the end of a spline if it has been supplied one.
 * @param PropertyChangedEvent 
 */
void AMapConnector::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Get the name of the property that changed
	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// Check if the property name that was changed matches the property we're looking for,
	// in this case the Spline Reference
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMapConnector, SplineRef))
	{
		// Check the Spline Reference is populated
		if (SplineRef)
		{
			// Get the Spline component and set the location of this actor to the end of that spline.
			const USplineComponent* SplineComponentRef = SplineRef->GetSpline();
			SetActorLocation(SplineComponentRef->GetLocationAtSplinePoint(SplineComponentRef->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World));
			SetActorRotation(SplineComponentRef->GetRotationAtSplinePoint(SplineComponentRef->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World));
		}
	}
}

/**
 * @brief Unreal Editor Function, called every time an actor is moved in the editor window.
 * This is used to keep the Map Connector at the end of of the spline reference, if it was populated.
 * @param bFinished Whether or not the movement action in editor is finishing or not
 */
void AMapConnector::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (SplineRef)
	{
		const USplineComponent* SplineComponentRef = SplineRef->GetSpline();
		SetActorLocation(SplineComponentRef->GetLocationAtSplinePoint(SplineComponentRef->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World));
		SetActorRotation(SplineComponentRef->GetRotationAtSplinePoint(SplineComponentRef->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World));
	}
}
#endif
