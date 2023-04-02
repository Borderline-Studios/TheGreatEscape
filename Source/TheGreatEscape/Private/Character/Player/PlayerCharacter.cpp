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
#include "NiagaraCommon.h"
#include "ParticleHelper.h"
#include "Chaos/ImplicitObject.h"




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
 // Position the camera
	

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	//Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	
	MuzzleSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MuzzleSphere"));
	MuzzleSphere->SetOnlyOwnerSee(true);
	MuzzleSphere->SetupAttachment(GetCapsuleComponent());
	MuzzleSphere->InitSphereRadius(50.0f);
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
		UGameplayStatics::PlaySoundAtLocation(this, HitSFX[FMath::RandRange(0,4)], GetActorLocation(), GetActorRotation(), 0.3);
		
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
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FVector ActorLoc = GetActorLocation();
	FRotator ActorRot = GetActorRotation();
	
	SetActorLocationAndRotation(FVector(ActorLoc.X,ActorLoc.Y, ActorLoc.Z - 100), FRotator(ActorRot.Pitch, ActorRot.Yaw, ActorRot.Roll + 5.0f));

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraFade(0.0f, 1.0f, 1.0f, FColor::Black, true, true);

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

void APlayerCharacter::LoadLevel()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu_01"));
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


