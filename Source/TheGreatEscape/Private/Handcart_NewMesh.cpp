// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2023 Media Design School
//
// File Name   :	Handcard_NewMesh.cpp
// Description :	Contains implementation of Handcart_NewMesh functionality. Should port this into a better named class.
// Author      :	Borderline Studios - Jake Laird
// Mail        :	jake.laird@mds.ac.nz

#include "Handcart_NewMesh.h"
#include "Kismet/GameplayStatics.h"
#include "TheGreatEscape/TheGreatEscapeCharacter.h"

/**
 * @brief
 * Default constructor for this object. Sets default values.
 */
AHandcart_NewMesh::AHandcart_NewMesh()
{
// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Setup
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = SceneRoot;

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Base"));
	Base->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> BaseObj(TEXT("StaticMesh'/Game/Production/Train/Art/Handcart/Handcart_Merged_Platform.Handcart_Merged_Platform'"));
	Base->SetStaticMesh(BaseObj.Object);
	Base->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	//Base->SetWorldScale3D(FVector(5.0f, 5.0f, 0.25f));

	Maglifts = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Maglifts"));
	Maglifts->SetupAttachment(Base);
	ConstructorHelpers::FObjectFinder<UStaticMesh> MagliftObj(TEXT("StaticMesh'/Game/Production/Train/Art/Handcart/Handcart_Merged_Maglifts.Handcart_Merged_Maglifts'"));
	Maglifts->SetStaticMesh(MagliftObj.Object);
	Maglifts->SetRelativeLocation(FVector(0.0f, 368.0f, 0.0f));
	
	Handle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Handle"));
	Handle->SetupAttachment(Base);
	ConstructorHelpers::FObjectFinder<UStaticMesh> HandleObj(TEXT("StaticMesh'/Game/Production/Train/Art/Handcart/Handcart_Merged_Handles.Handcart_Merged_Handles'"));
	Handle->SetStaticMesh(HandleObj.Object);
	Handle->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	// Handle->SetWorldScale3D(FVector(1.5f, 0.05f, 0.05f));
	Handle->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// End Mesh Setup
	// Start Collision Setup
	FrontCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FrontCollider"));
	FrontCollision->SetupAttachment(RootComponent);
	FrontCollision->SetRelativeLocation(FVector(80.0f, 0.0f, 150.0f));
	FrontCollision->SetWorldScale3D(FVector(1.25f, 3.0f, 2.0f));
	
	BackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BackCollider"));
	BackCollision->SetupAttachment(RootComponent);
	BackCollision->SetRelativeLocation(FVector(-80.0f, 0.0f, 150.0f));
	BackCollision->SetWorldScale3D(FVector(1.25f, 3.0f, 2.0f));
	// End Collision Setup
	// Start Collision Assignment
	FrontCollision->OnComponentBeginOverlap.AddDynamic(this, &AHandcart_NewMesh::BeginFrontOverlap);
	FrontCollision->OnComponentEndOverlap.AddDynamic(this, &AHandcart_NewMesh::EndFrontOverlap);
	
	BackCollision->OnComponentBeginOverlap.AddDynamic(this, &AHandcart_NewMesh::BeginBackOverlap);
	BackCollision->OnComponentEndOverlap.AddDynamic(this, &AHandcart_NewMesh::EndBackOverlap);
	// End Collision Assignment
}

/**
 * @brief
 * Ensures that the HandCart has it's mesh populated. If not, prevent sit from calling or trying to work on anything that 
 */
void AHandcart_NewMesh::BeginPlay()
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
void AHandcart_NewMesh::Tick(float DeltaTime)
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
		const float HandleYMod = FMath::Sin(2 * TimeSinceStart);
		Handle->SetRelativeRotation(FRotator(0.0f, 0.0f, HandleYMod * 30));

		float Progress = EngineStart + FMath::Lerp(0, SplineRef->GetSplineLength(), TimerTrack);

		if (Progress >= SplineRef->GetSplineLength())
		{
			Progress -= SplineRef->GetSplineLength();
		}

		SetActorTransform(SplineRef->GetTransformAtDistanceAlongSpline(Progress, ESplineCoordinateSpace::World));
	}
}

void AHandcart_NewMesh::BeginFrontOverlap(
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

void AHandcart_NewMesh::EndFrontOverlap(
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

void AHandcart_NewMesh::BeginBackOverlap(
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

void AHandcart_NewMesh::EndBackOverlap(
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