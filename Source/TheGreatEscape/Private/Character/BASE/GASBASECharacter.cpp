// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : GASBASECharacter.cpp
// Description : Contains all of the functionality for the GAS Character base class for usage by the character 
// Author      :  Borderline Studios - Jacob MacLean
// Mail        : Jacob.MacLean@mds.ac.nz

//Base Includes
#include "Character/BASE/GASBASECharacter.h"

//Constructor
AGASBASECharacter::AGASBASECharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating the AbilitySystemCompoenent and setting its replication (Require for GAS back end even though our game isn't Multiplayer)
	AbilitySystemComponent = CreateDefaultSubobject<UQRAbilitySystemComponent>(TEXT("Ability System"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//Creating the Attribute set for the player
	Attributes = CreateDefaultSubobject<UQRAttributeSet>(TEXT("Attributes"));

}

/**
 * @brief GAS built in Function. helps sort out damage for gameplay effects (Back end Stuff)
 * @param DamageAmount - Input damage amount
 * @param HitInfo - Info of the thing being hit
 * @param DamageTags - Checks for type of damage tag (e.g Poison, Slashing, Piercing)
 * @param InstigatorCharacter - gets the character the called the HandleDamage() function
 * @param DamagerCauser - Get the Character/actor doing the damage
 */
void AGASBASECharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo,
                                     const FGameplayTagContainer& DamageTags, ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamagerCauser)
{
	//Calls UE built-in function OnDamaged()
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamagerCauser); 
}

/**
 * @brief Handles the health change for the GAS character (GAS built in function)
 * @param Deltavalue - Value the is changing
 * @param EventTags -Checks for any events that would change the value
 */
void AGASBASECharacter::HandleHealthChanged(float Deltavalue, const FGameplayTagContainer& EventTags)
{
	//Checks if the abilities exist
	if(bAbilitiesInitalized)
	{
		//Calls UE built-in function ONHealthChanged() 
		OnHealthChanged(Deltavalue, EventTags); 
	}	
}

/**
 * @brief UE Built in function that the activates when the player is possessed
 * @param NewController - Get the controller of the thing being possessed
 */
void AGASBASECharacter::PossessedBy(AController* NewController)
{

	//Checks if the abilitySystemComponent Exists
	if(AbilitySystemComponent)
	{
		//Initializing the actors/characters ability info
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	//Adds the default/startup abilities to the player
	AddStartupGameplayAbilities();
	Super::PossessedBy(NewController);

}

/**
 * @brief Built-in Unreal function to replicate the player state on the server.
 * Need for GAS back end functionality despite the game not being multiplayer
 */
void AGASBASECharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Initializes the ability actor info for the Character/Actor
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	//Checks if the ACS and IC are Vaild/Exist
	if (AbilitySystemComponent && InputComponent)
	{
		//Binds the Input for GAS.
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID",
			static_cast<int32>(EGASAbilityInputID::Confirm),static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

/**
 * @brief Adds the default abilities and effects inside the Arrays in the header to the player
 */
void AGASBASECharacter::AddStartupGameplayAbilities()
{
	//Checks if ASC Exists/IsVaild
	check(AbilitySystemComponent);
	//Checks if the Object has Server Authority to give abilities and the abilities haven't already been activated/Initalised
	if(GetLocalRole() == ROLE_Authority && !bAbilitiesInitalized)
	{
		//Grants Abilities, but only on Server (GAS requires a bunch of server replication stuff even if game isn't multiplayer)
		for(TSubclassOf<UQRGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			if(StartupAbility)
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
				StartupAbility, 1,
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}

	//Grants Effect, but only on server  (GAS requires a bunch of server replication stuff even if game isn't multiplayer)
	for (const TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);

		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get(), AbilitySystemComponent);
		}

		//Sets the ability initalized check to be true
		bAbilitiesInitalized = true;
	}
}

/**
 * @brief Gets the character ASC and returns it, used for accessing things the like attribute set and ability etc.
 * @return The Characters Ability System Component
 */
UAbilitySystemComponent* AGASBASECharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AGASBASECharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGASBASECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * @brief UE built in movement functions
 * @param Val - speed input
 */
void AGASBASECharacter::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}
/**
 * @brief UE built in movement functions
 * @param Val - speed input
 */
void AGASBASECharacter::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}
/**
 * @brief UE built in movement functions
 * @param Val - speed input
 */
void AGASBASECharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}
/**
 * @brief UE built in movement functions
 * @param Val - speed input
 */
void AGASBASECharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}


/**
 * @brief Setsup the input for the player to interact with GAS and the Ability IDs
 * @param PlayerInputComponent Get the players input component
 */
void AGASBASECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGASBASECharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGASBASECharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AGASBASECharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AGASBASECharacter::LookUpAtRate);


	//Same code to the OnRep_Player State, just in case the Input set up is called before the repstate.
	//(Hard to tell which is first so done as a precaution
	if (AbilitySystemComponent && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID",
			static_cast<int32>(EGASAbilityInputID::Confirm),static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

