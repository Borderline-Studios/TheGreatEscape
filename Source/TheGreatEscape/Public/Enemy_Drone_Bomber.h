// // Bachelor of Software Engineering// Media Design School// Auckland// New Zealand// // (c) 2022 Media Design School//// File Name   : // Description : // Author      :  Borderline Studios - (person(s) working on file)// Mail        : 

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Enemy_Drone_Bomber.generated.h"

class APlayerCharacter;
UCLASS()
class THEGREATESCAPE_API AEnemy_Drone_Bomber : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy_Drone_Bomber();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* DetectionSphere;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* HitSphere;
	
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* ExplodeSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DeathVFX;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TargetPlayer();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Explode(FVector Location);

	UFUNCTION(BlueprintCallable)
	void PostHitProcress();

	UFUNCTION()
	APlayerCharacter* GetPlayerReference();

};
