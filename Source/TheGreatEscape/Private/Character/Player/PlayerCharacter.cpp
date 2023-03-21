// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : PlayerCharacter.cpp
// Description : Contains the functionality for the player character.
// Author      :  Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

//UE includes
#include "Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/InputSettings.h"
//GAS Includes
#include "AbilitySystemBlueprintLibrary.h"


APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(40.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	//Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Check if the players health is less than or equal to 0 then kill
	//todo(Jacob) Set up a better system for this
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);
		bool Found;
		float Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetHealthAttribute(), Found);
	if(Found)
	{
		if (Value < 30.0f)
		{
			CallVignette();
		}
		
		if (Value <= 0.0f)
		{
			StartDeath();
		}
	}
}

/**
 * @brief called when the players health is depleted
 */
void APlayerCharacter::StartDeath()
{
	//Displables the player input
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

/**
 * @brief Returns the FPCC - used for access to line traces in other classes and functions
 * @return FirstPersonCharacterComponenet
 */
UCameraComponent* APlayerCharacter::GetFirstPersonCameraComponent()
{
	return FirstPersonCameraComponent;
}


/**
 * @brief Casts and returns a player reference for other classes to use
 * @return APlayerCharacter pointer Reference
 */
APlayerCharacter* APlayerCharacter::GetPlayerReference()
{
	APlayerCharacter* PlayerCharacter = this;
	return PlayerCharacter;
}

void APlayerCharacter::SetResources(int ValueToChange, int NewValue)
{
	ValueToChange = NewValue;
	NumScrap = ValueToChange;
	
}
