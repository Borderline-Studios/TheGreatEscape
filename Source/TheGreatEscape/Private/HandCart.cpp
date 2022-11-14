// Fill out your copyright notice in the Description page of Project Settings.
#include "HandCart.h"

#include "Kismet/GameplayStatics.h"
#include "TheGreatEscape/TheGreatEscapeCharacter.h"

// Sets default values
AHandCart::AHandCart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Setup
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Base"));
	Base->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	Base->SetStaticMesh(MeshObj.Object);
	Base->SetWorldScale3D(FVector(5.0f, 5.0f, 0.25f));

	Middle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Pole"));
	Middle->SetupAttachment(RootComponent);
	Middle->SetStaticMesh(MeshObj.Object);
	Middle->SetWorldScale3D(FVector(0.1f, 0.1f, 2.0f));

	Handle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Handle"));
	Handle->SetupAttachment(RootComponent);
	Handle->SetStaticMesh(MeshObj.Object);
	Handle->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	Handle->SetWorldScale3D(FVector(1.5f, 0.05f, 0.05f));
	Handle->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform LGrip"));
	FGrip->SetupAttachment(Handle);
	FGrip->SetStaticMesh(MeshObj.Object);
	FGrip->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	FGrip->SetWorldScale3D(FVector(0.04f, 10.0f, 1.0f));
	FGrip->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BGrip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform RGrip"));
	BGrip->SetupAttachment(Handle);
	BGrip->SetStaticMesh(MeshObj.Object);
	BGrip->SetRelativeLocation(FVector(-50.0f, 0.0f, 0.0f));
	BGrip->SetWorldScale3D(FVector(0.04f, 10.0f, 1.0f));
	BGrip->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetArrowColor(FColor::Purple);
	Arrow->SetHiddenInGame(false);
	Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	// End Mesh Setup
	// Start Collision Setup
	FrontCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FrontCollider"));
	FrontCollision->SetupAttachment(RootComponent);
	FrontCollision->SetRelativeLocation(FVector(130.0f, 0.0f, 120.0f));
	FrontCollision->SetWorldScale3D(FVector(3.5f, 7.75f, 3.0f));
	FrontCollision->SetHiddenInGame(false);
	
	BackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BackCollider"));
	BackCollision->SetupAttachment(RootComponent);
	BackCollision->SetRelativeLocation(FVector(-130.0f, 0.0f, 120.0f));
	BackCollision->SetWorldScale3D(FVector(3.5f, 7.75f, 3.0f));
	BackCollision->SetHiddenInGame(false);
	// End Collision Setup
	// Start Collision Assignment
	FrontCollision->OnComponentBeginOverlap.AddDynamic(this, &AHandCart::BeginFrontOverlap);
	FrontCollision->OnComponentEndOverlap.AddDynamic(this, &AHandCart::EndFrontOverlap);
	
	BackCollision->OnComponentBeginOverlap.AddDynamic(this, &AHandCart::BeginBackOverlap);
	BackCollision->OnComponentEndOverlap.AddDynamic(this, &AHandCart::EndBackOverlap);
	// End Collision Assignment
}

// Called when the game starts or when spawned
void AHandCart::BeginPlay()
{
	Super::BeginPlay();	

	// Spline setup
	if (!TrackActorRef) {}
	else if (USplineComponent* TempSplineRef = Cast<USplineComponent>(TrackActorRef->GetRootComponent()); TempSplineRef != nullptr)
	{
		SplineRef = TempSplineRef;
		PlayerRef = UGameplayStatics::GetActorOfClass(GetWorld(), ATheGreatEscapeCharacter::StaticClass());
		GEngine->AddOnScreenDebugMessage(15, 5, FColor::Green, FString::Printf(TEXT("Player Ref populated: %d"), PlayerRef != nullptr));

		EngineStart = ((FMath::Rand() / RAND_MAX) * SplineRef->GetSplineLength());
		SetActorTransform(SplineRef->GetTransformAtDistanceAlongSpline(EngineStart, ESplineCoordinateSpace::World));

		bIsInitialised = true;
	}
}

// Called every frame
void AHandCart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInitialised && (IsFrontCollided || IsBackCollided))
	{
		if (IsFrontCollided)
		{
			TimeSinceStart -= DeltaTime;
		}
		else if (IsBackCollided)
		{
			TimeSinceStart += DeltaTime;
		}
		
		if (TimeSinceStart <= 0)
		{
			TimeSinceStart = TempTTC;
		}
		else if (TimeSinceStart >= TempTTC)
		{
			TimeSinceStart = 0;
		}

		const float TimerTrack = TimeSinceStart / TempTTC;

		// Calculate the rotation of the Handle
		const float HandleYMod = FMath::Sin(5 * TimeSinceStart);
		Handle->SetRelativeRotation(FRotator(HandleYMod * 30, 0.0f, 0.0f));

		float Progress = EngineStart + FMath::Lerp(0, SplineRef->GetSplineLength(), TimerTrack);

		if (Progress >= SplineRef->GetSplineLength())
		{
			Progress -= SplineRef->GetSplineLength();
		}

		SetActorTransform(SplineRef->GetTransformAtDistanceAlongSpline(Progress, ESplineCoordinateSpace::World));
	}
}

void AHandCart::BeginFrontOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == PlayerRef)
	{
		IsFrontCollided = true;
	}
}

void AHandCart::EndFrontOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor == PlayerRef)
	{
		IsFrontCollided = false;
	}
}

void AHandCart::BeginBackOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == PlayerRef)
	{
		IsBackCollided = true;
	}
}

void AHandCart::EndBackOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor == PlayerRef)
	{
		IsBackCollided = false;
	}
}

