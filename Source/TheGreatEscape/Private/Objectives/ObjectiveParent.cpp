// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   :
// Description :
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        : 

#include "Objectives/ObjectiveParent.h"

#include "Character/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Static Variable Declarations
USplineComponent* AObjectiveParent::SplineRef;

// Sets default values
AObjectiveParent::AObjectiveParent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!SplineRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing a Spline Reference"));
	}

	Detector = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = Detector;
	Detector->SetWorldScale3D(FVector(0.25f, 3.0f, 3.0f));

	if (!PlayerRef)
	{
		PlayerRef = UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass());
	}
}

// Called when the game starts or when spawned
void AObjectiveParent::BeginPlay()
{
	Super::BeginPlay();

	if (!PlayerRef)
	{
		PlayerRef = UGameplayStatics::GetActorOfClass(this, APlayerCharacter::StaticClass());
	}
}

// Called every frame
void AObjectiveParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * @brief
 * Allows for external population of the spline ref but only if it has not already been provided.
 * @param NewRef The spline to be referenced by the objectives 
 */
void AObjectiveParent::PopulateSplineRef(USplineComponent* NewRef)
{
	if (!SplineRef)
	{
		SplineRef = NewRef;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The spline reference has already been populated!"));
	}
}
