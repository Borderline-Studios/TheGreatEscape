// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2022 Media Design School
// File Name   :
// Description :
// Author      :  Borderline Studios - (person(s) working on file)
// Mail        :

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "TrainCarriage.h"

#include "AbilitySystemInterface.h"
#include "QRAttributeSet.h"
#include "QRAbilitySystemComponent.h"
#include "QRGameplayAbility.h"

#include "TrainEngine.generated.h"

class UQRGameplayAbility;
class UQRAbilitySystemComponent;
class UQRAttributeSet;

// Enums
UENUM(BlueprintType)
enum class ETrainSpeed : uint8
{
    Slow,
    Standard,
    Fast
};

UCLASS()
class THEGREATESCAPE_API ATrainEngine : public AActor
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ATrainEngine();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
    TArray<TSubclassOf<UQRGameplayAbility>> GameplayAbilities;

    UPROPERTY()
    uint8 bAbilitiesInitalized:1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
    TObjectPtr<UQRAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UQRAttributeSet> Attributes;

    //Functions
    UFUNCTION(BlueprintImplementableEvent)
    void OnDamaged(float DamageAmount, const FHitResult& HitInfo,
                   const struct FGameplayTagContainer& DamageTags,
                   ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamagerCauser);

    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float Deltavalue, const struct FGameplayTagContainer& EventTags);

    virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo,
                   const struct FGameplayTagContainer& DamageTags,
                   ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamagerCauser);

    virtual void HandleHealthChanged(float Deltavalue, const struct FGameplayTagContainer& EventTags);
    
    void AddStartupGameplayAbilities();
    
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Custom section containing everything added beyond the default Unreal code
    
    // Track Changing
    UPROPERTY(BlueprintReadWrite)
    bool bShouldChangeTracks = true;
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite)        // The actor passed in from the editor
    AActor* TrackActorRef;

    // Functions
    UFUNCTION(BlueprintCallable)
    void ToggleTrainStop();

    UFUNCTION(BlueprintCallable)
    void SetTrainSpeed(ETrainSpeed NewSpeed);

protected:


private:    
    // Variables
    // Obtaining the spline for the train to follow
    USplineComponent* TrackSplineRef;
    float SplineLength;
    UPROPERTY(EditInstanceOnly)
    int TimeToComplete = 30;
    UPROPERTY(EditInstanceOnly)
    int StartDelayTime;
    
    float TimeSinceStart;
    bool bHasStartedMoving = false;
    bool bTrainMoving = true;
    float TrainSpeedModifier = 1.0f;
    
    // Keeping track of the entire track (pun intended)
    TArray<class ASplineTrack*> CompleteTrackRefs;

    // Holds the Shapes used to show the engine
    USceneComponent* SceneRoot;

    UPROPERTY(EditInstanceOnly)
    UStaticMeshComponent* EngineMesh;
    UStaticMeshComponent* Box;

    UPROPERTY(EditInstanceOnly)
    UArrowComponent* ArrowComp;

    // CARRIAGE HANDLING SECTION
    static TStaticArray<UStaticMesh*, 4> StaticMeshRefs;
    TArray<ATrainCarriage*> CarriageRefs;
    UPROPERTY(EditInstanceOnly)
    int CarriageCount = 0;

    float EngineStart = 0;

    UPROPERTY(EditInstanceOnly)
    TArray<int> StopIndices;
    TArray<bool> StoppedAtIndex;

    // Functions
};
