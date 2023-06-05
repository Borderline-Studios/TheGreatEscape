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
#include "Components/SphereComponent.h"
#include "GameFramework/InputSettings.h"
//GAS Includes
#include "AbilitySystemBlueprintLibrary.h"




APlayerCharacter::APlayerCharacter()
{
	bRifleEquipped = false;
	bRiflePickedUp = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(40.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
 // Position the camera
	

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	RevolverMesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	RevolverMesh1P->SetOnlyOwnerSee(true);
	RevolverMesh1P->SetupAttachment(FirstPersonCameraComponent);
	RevolverMesh1P->bCastDynamicShadow = false;
	RevolverMesh1P->CastShadow = false;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	RifleMesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleMesh1P"));
	RifleMesh1P->SetOnlyOwnerSee(true);
	RifleMesh1P->SetupAttachment(FirstPersonCameraComponent);
	RifleMesh1P->bCastDynamicShadow = false;
	RifleMesh1P->CastShadow = false;
	//RevolverMesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	//RevolverMesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	
	MuzzleSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MuzzleSphere"));
	MuzzleSphere->SetOnlyOwnerSee(true);
	MuzzleSphere->SetupAttachment(GetCapsuleComponent());
	MuzzleSphere->InitSphereRadius(50.0f);

	RifleMuzzleSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RifleMuzzleSphere"));
	RifleMuzzleSphere->SetOnlyOwnerSee(true);
	RifleMuzzleSphere->SetupAttachment(GetCapsuleComponent());
	RifleMuzzleSphere->InitSphereRadius(50.0f);
}

void APlayerCharacter::PostHitProcess()
{
	
	//Check if the players health is less than or equal to 0 then kill
	//todo(Jacob) Set up a better system for this
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);
	bool Found;
	float Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetHealthAttribute(), Found);
	if (Found)
	{
		if (!HitSFX.IsEmpty())
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSFX[FMath::RandRange(0,2)], GetActorLocation(), GetActorRotation(), 0.3);
		}

		
		if (Value <= 30.0f)
		{
			CallVignette();
		}
		else if (Value > 30.0f)
		{
			DisableVignette();
		}


		if (Value <= 0.0f)
		{
			PostDeathProcess();
		}
	}
}

void APlayerCharacter::PostDeathProcess()
{
	//Displables the player input
	DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->StopMovement();
	GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight()/4);
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(0.0f, 1.0f, 1.0f, FColor::Black, false, true);

	FTimerHandle DeathTimer;

	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &APlayerCharacter::LoadLevel, 2.0f, false);
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

UCameraComponent* APlayerCharacter::GetFirstPersonCameraComponent()
{
	return FirstPersonCameraComponent;
}

void APlayerCharacter::IncrementBatteryCount()
{
	PlayerBatteryCount++;
}

void APlayerCharacter::DecrementBatteryCount()
{
	if (PlayerHasBattery())
	{
		PlayerBatteryCount--;
	}
}

int APlayerCharacter::GetPlayerBatteryCount() const
{
	return PlayerBatteryCount;
}

bool APlayerCharacter::PlayerHasBattery()
{
	return PlayerBatteryCount > 0;
}

void APlayerCharacter::LoadLevel()
{
	RootComponent->SetWorldLocation(RespawnLocation);
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld() ,0));
	GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*4);
	ReApplyPassives();
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(1.0f, 0.0f, 1.0f, FColor::Black, false, true);
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


