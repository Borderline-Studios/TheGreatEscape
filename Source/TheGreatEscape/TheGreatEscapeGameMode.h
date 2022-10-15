// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheGreatEscapeGameMode.generated.h"

UCLASS(minimalapi)
class ATheGreatEscapeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATheGreatEscapeGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumOfEnemiesAlive;
};



