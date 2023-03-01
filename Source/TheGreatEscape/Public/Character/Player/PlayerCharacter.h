// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BASE/GASBASECharacter.h"
#include "PlayerCharacter.generated.h"


class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
/**
 * 
 */
UCLASS(Config=Game)
class THEGREATESCAPE_API APlayerCharacter : public AGASBASECharacter
{
	GENERATED_BODY()
public:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	APlayerCharacter();

	UCameraComponent* GetFirstPersonCameraComponent();
};
