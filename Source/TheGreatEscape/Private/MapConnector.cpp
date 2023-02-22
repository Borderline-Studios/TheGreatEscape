// Fill out your copyright notice in the Description page of Project Settings.


#include "MapConnector.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "TheGreatEscape/TheGreatEscapeCharacter.h"

// Sets default values
AMapConnector::AMapConnector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	CollisionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionDetection"));
	CollisionDetection->SetupAttachment(RootComponent);

	CollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &AMapConnector::BeginFrontOverlap);

}

// Called when the game starts or when spawned
void AMapConnector::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = UGameplayStatics::GetActorOfClass(GetWorld(), ATheGreatEscapeCharacter::StaticClass());
}

// Called every frame
void AMapConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEndingMap)
	{
		FVector temp = PlayerCamRef->GetRelativeLocation();
		PlayerCamRef->SetWorldLocation(FVector(temp.X, temp.Y, temp.Z + 1));
	}

}

void AMapConnector::BeginFrontOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	if (OtherActor == PlayerRef)
	{
		// Insert other boolean checks here

		
		//IsEndingMap = true;

		// Spawn the second camera
		ACameraActor* NewCam = Cast<ACameraActor>(GetWorld()->SpawnActor(ACameraActor::StaticClass()));
		NewCam->SetActorLocation(GetActorLocation() + FVector(0, 0, ZHeight));
		NewCam->GetCameraComponent()->SetConstraintAspectRatio(false);
		// Set NewCam Rotation

		// Tween the camera using method one from that video
		APlayerController* temp = UGameplayStatics::GetPlayerController(this, 0);
		temp->SetViewTargetWithBlend(NewCam, BlendTime);

		// Load the next level
		FTimerHandle LoadLevelHandle;
		GetWorld()->GetTimerManager().SetTimer(LoadLevelHandle, [&]()
		{
			UGameplayStatics::OpenLevel(this, FName("Jake_Test"));
		}, BlendTime * 2, false);
	}
}

