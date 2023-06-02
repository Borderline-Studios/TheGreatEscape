// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy_Drone.generated.h"

class APlayerCharacter;

UCLASS()
class THEGREATESCAPE_API AEnemy_Drone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_Drone();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BodyMesh;
	
    UPROPERTY(EditDefaultsOnly)
    UStaticMeshComponent* GunMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* GunBaseMesh;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* RotorMesh;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* DetectionSphere;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* HitSphere;

	UPROPERTY(BlueprintReadWrite)
	bool bInRange = false;

	UFUNCTION(BlueprintCallable)
	void LookAtPlayer(bool Interp);
	
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerReference();

	UFUNCTION(BlueprintCallable)
	void TrackPlayer();

	UFUNCTION(BlueprintCallable)
	void StopTrackPlayer();

	UFUNCTION(BlueprintImplementableEvent)
	void LazerTracePlayer();

	UFUNCTION(BlueprintImplementableEvent)
	void CheckLineToPlayer();

	UFUNCTION()
	void PostHitProcess();

	UFUNCTION(BlueprintCallable)
	void ChargeSystemTimer();

	UPROPERTY()
	bool bKeepTracking = true;

	UPROPERTY(BlueprintReadOnly)
	int NumTracks = 0;

	UPROPERTY()
	int TracksNeeded = 4;

	UPROPERTY(BlueprintReadWrite)
	float PitchMod = 1.0f;

	FTimerHandle ChargeTimer;

	UPROPERTY(BlueprintReadWrite)
	float Health = 10.0f;

	FTimerHandle TrackTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VFX)
	UNiagaraSystem* ExplodeSystem;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
