// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 


#include "Enemy_Drone_Bomber.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy_Drone_Bomber::AEnemy_Drone_Bomber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = StaticMesh;

	DetectionSphere = CreateDefaultSubobject<USphereComponent>("DetectionSphere");
	DetectionSphere->InitSphereRadius(2000.0f);
	DetectionSphere->SetupAttachment(RootComponent);

	HitSphere = CreateDefaultSubobject<USphereComponent>("HitSphere");
	HitSphere->InitSphereRadius(50.0f);
	HitSphere->SetupAttachment(RootComponent);

	ExplodeSphere = CreateDefaultSubobject<USphereComponent>("ExplodeSphere");
	ExplodeSphere->InitSphereRadius(200.0f);
	ExplodeSphere->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("LockOnMovementComponent");
}

// Called when the game starts or when spawned
void AEnemy_Drone_Bomber::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_Drone_Bomber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_Drone_Bomber::PostHitProcress()
{
	Destroy();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathVFX, RootComponent->GetComponentLocation(), FRotator(0,0,0), FVector(0.2, 0.2,0.2));
}

APlayerCharacter* AEnemy_Drone_Bomber::GetPlayerReference()
{
	//Casts to the player and assigns the pointer reference to the CharacterRef Varaible
	APlayerCharacter* CharacterRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//Returns Character Ref
	return CharacterRef;
}

