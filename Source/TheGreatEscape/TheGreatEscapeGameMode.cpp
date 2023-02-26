// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheGreatEscapeGameMode.h"
#include "TheGreatEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheGreatEscapeGameMode::ATheGreatEscapeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/QRPlayer.QRPlayer"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;
}
