// Fill out your copyright notice in the Description page of Project Settings.


#include "MapConnector.h"

#include "Camera/CameraActor.h"
#include "Character/QRCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TheGreatEscape/TheGreatEscapeCharacter.h"

// Sets default values
AMapConnector::AMapConnector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionDetection = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionDetection"));
	CollisionDetection->SetupAttachment(RootComponent);
	CollisionDetection->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));

	RootComponent = CollisionDetection;

	CollisionDetection->OnComponentBeginOverlap.AddDynamic(this, &AMapConnector::BeginFrontOverlap);

}

// Called when the game starts or when spawned
void AMapConnector::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = UGameplayStatics::GetActorOfClass(GetWorld(), AQRCharacter::StaticClass());
}

// Called every frame
void AMapConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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

		// Spawn the second camera
		ACameraActor* PlayerCam = Cast<ACameraActor>(GetWorld()->SpawnActor(ACameraActor::StaticClass()));
		PlayerCam->SetActorLocation(PlayerRef->GetActorLocation());
		PlayerCam->GetCameraComponent()->SetConstraintAspectRatio(false);
		
		ACameraActor* TopCam = Cast<ACameraActor>(GetWorld()->SpawnActor(ACameraActor::StaticClass()));
		TopCam->SetActorLocation(GetActorLocation() + FVector(0, 0, ZHeight));
		// Set NewCam Rotation

		// Tween the camera using method one from that video
		APlayerController* temp = UGameplayStatics::GetPlayerController(this, 0);
		temp->SetViewTargetWithBlend(PlayerCam, 0);
		temp->SetViewTargetWithBlend(TopCam, BlendTime);

		// Set for the camera to fade during the blend time, maybe through the use of another lambda TimerHandle

		// Load the next level
		FTimerHandle LoadLevelHandle;
		GetWorld()->GetTimerManager().SetTimer(LoadLevelHandle, [&]()
		{
			if (UseWorldRef)
			{
				UGameplayStatics::OpenLevelBySoftObjectPtr(this, NextWorldRef);
			}
			else
			{
				UGameplayStatics::OpenLevel(this, NextMapName);
			}
			
		}, BlendTime * 2, false);
	}
}

