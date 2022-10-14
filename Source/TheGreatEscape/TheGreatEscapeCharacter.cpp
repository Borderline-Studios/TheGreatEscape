// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheGreatEscapeCharacter.h"

#include <string>

#include "TheGreatEscapeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
//#include "Enemy.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Kismet/GameplayStatics.h"

//GAS INCLUDES
#include "GASAbilitySystemComponent.h"
#include "GASAttributeSet.h"
#include "GASGameplayAbility.h"
#include <GameplayEffectTypes.h>




//////////////////////////////////////////////////////////////////////////
// ATheGreatEscapeCharacter

ATheGreatEscapeCharacter::ATheGreatEscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

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
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	AbilitySystemComponent = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	Attributes = CreateDefaultSubobject<UGASAttributeSet>("Attributes");
}

void ATheGreatEscapeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void ATheGreatEscapeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &ATheGreatEscapeCharacter::OnPrimaryAction);
	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &ATheGreatEscapeCharacter::OnMelee);
	

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ATheGreatEscapeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ATheGreatEscapeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ATheGreatEscapeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ATheGreatEscapeCharacter::LookUpAtRate);

	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID",
			static_cast<int32>(EGASAbilityInputID::Confirm),static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
	
}

UAbilitySystemComponent* ATheGreatEscapeCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATheGreatEscapeCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1 , EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATheGreatEscapeCharacter::GiveAbilities()
{
		if (!bAbilitiesAdded)
		{
			bAbilitiesAdded = true;
			if (HasAuthority() && AbilitySystemComponent)
			{
				for (TSubclassOf<UGASGameplayAbility>& StartUpAbility : DefaultAbilities)
				{
					AbilitySystemComponent->GiveAbility(
						FGameplayAbilitySpec(StartUpAbility, 1 , static_cast<int32>(StartUpAbility.GetDefaultObject()->AbilityInputID), this));
				}
			}
		}
}

void ATheGreatEscapeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	GiveAbilities();
}

void ATheGreatEscapeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	if (AbilitySystemComponent && InputComponent)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, TEXT("ASC and IC valid"));
		}
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID",
			static_cast<int32>(EGASAbilityInputID::Confirm),static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 150.0f, FColor::Yellow, TEXT("ASC and IC not valid"));
		}
	}

}

void ATheGreatEscapeCharacter::OnPrimaryAction()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);
	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * 18000);

	FCollisionQueryParams TraceParams;

	TraceParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, TraceParams);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.05, 0, 0.5);

	if (Hit.GetActor() != nullptr)
	{
		//AEnemy* Enemy = Cast<AEnemy>(Hit.GetActor());

		//if (Enemy)
		//{
			//if (GEngine)
			//{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, Hit.GetActor()->GetName());
				//bHitEnemy = true;
				//Enemy->DamageEnemy(GetActorLocation());
			//}
		//}
	}
}

void ATheGreatEscapeCharacter::OnMelee()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Loc, Rot);
	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * 150);

	FCollisionQueryParams TraceParams;

	TraceParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, TraceParams);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.05, 0, 0.5);

	if (Hit.GetActor() != nullptr)
	{
		//AEnemy* Enemy = Cast<AEnemy>(Hit.GetActor());

		//if (Enemy)
		//{
			//if (GEngine)
			//{
				//bHitEnemy = true;
				//Enemy->DamageEnemy(GetActorLocation());
			//}
		//}
	}
}

void ATheGreatEscapeCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ATheGreatEscapeCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void ATheGreatEscapeCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATheGreatEscapeCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATheGreatEscapeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ATheGreatEscapeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool ATheGreatEscapeCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATheGreatEscapeCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ATheGreatEscapeCharacter::EndTouch);

		return true;
	}

	return false;
}
