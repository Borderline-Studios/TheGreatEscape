// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Enemy_Drone.h"

#include "PrimitiveSceneInfo.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy_Drone::AEnemy_Drone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = BodyMesh;

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("GunMesh");
	GunMesh->SetupAttachment(RootComponent);

	GunBaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("GunBaseMesh");
	GunBaseMesh->SetupAttachment(RootComponent);

	RotorMesh = CreateDefaultSubobject<UStaticMeshComponent>("RotorMesh");
	RotorMesh->SetupAttachment(RootComponent);
	

	DetectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	DetectionSphere->SetupAttachment(RootComponent);
	DetectionSphere->InitSphereRadius(2000.0f);

	HitSphere = CreateDefaultSubobject<USphereComponent>("HitSphere");
	HitSphere->InitSphereRadius(50.0f);
	HitSphere->SetupAttachment(RootComponent);

}

void AEnemy_Drone::LookAtPlayer(bool Interp)
{
	FRotator LookAtRotation;
	FRotator CurrentLookRotation = GetActorRotation();
	FVector PlayerLocation = GetPlayerReference()->GetActorLocation();
	FVector DroneLocation = this->GetActorLocation();
	LookAtRotation = UKismetMathLibrary::FindLookAtRotation(DroneLocation, PlayerLocation);

	if (Interp)
	{
		SetActorRotation(FMath::RInterpTo(CurrentLookRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), 1.0f));
	}
	else
	{
		SetActorRotation(LookAtRotation);
	}

	
	if (bKeepTracking)
	{
		GetWorld()->GetTimerManager().SetTimer(TrackTimerHandle, this, &AEnemy_Drone::TrackPlayer, 1.0f, true);
		bKeepTracking = false;
	}

}

APlayerCharacter* AEnemy_Drone::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//Returns Character Ref
	return CharacterRef;
}

void AEnemy_Drone::TrackPlayer()
{
	NumTracks++;
	if (NumTracks >= TracksNeeded)
	{
		GetPlayerReference()->PostDeathProcess();
		NumTracks = 0;
	}
}

void AEnemy_Drone::StopTrackPlayer()
{
	GetWorld()->GetTimerManager().ClearTimer(TrackTimerHandle);
	NumTracks = 0;
	bKeepTracking = true;
}

// Called when the game starts or when spawned
void AEnemy_Drone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_Drone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInRange)
	{
		LookAtPlayer(false);
		LazerTracePlayer();
	}

}

