// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY()
	bool bKeepTracking = true;

	UPROPERTY()
	int NumTracks = 0;

	UPROPERTY()
	int TracksNeeded = 3;

	FTimerHandle TrackTimerHandle;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
