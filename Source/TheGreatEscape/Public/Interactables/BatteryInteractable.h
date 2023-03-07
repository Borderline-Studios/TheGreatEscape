// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BatteryInteractable.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;



UCLASS()
class THEGREATESCAPE_API ABatteryInteractable : public AActor
{
	GENERATED_BODY()
	
public:
	//Static Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPickedUp = false;
	
	// Sets default values for this actor's properties
	ABatteryInteractable();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetPickedUp();
	

};
