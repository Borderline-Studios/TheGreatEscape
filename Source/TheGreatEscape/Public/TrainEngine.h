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

#include "Train/TrainCarParent.h"
#include "TrainCarriage.h"

#include "AbilitySystemInterface.h"
#include "QRAttributeSet.h"
#include "QRAbilitySystemComponent.h"
#include "QRGameplayAbility.h"
#include "Components/TimelineComponent.h"

#include "TrainEngine.generated.h"

class ATrainControlls;
class ASplineTrack;
class UQRGameplayAbility;
class UQRAbilitySystemComponent;
class UQRAttributeSet;

UCLASS()
class THEGREATESCAPE_API ATrainEngine : public ATrainCarParent
{
    GENERATED_BODY()
    
public:    
    // Sets default values for this actor's properties
    ATrainEngine();

#pragma region GAS DEFINITIONS
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
                   ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamageCauser);

    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float Deltavalue, const struct FGameplayTagContainer& EventTags);

    virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo,
                   const struct FGameplayTagContainer& DamageTags,
                   ATheGreatEscapeCharacter* InstigatorCharacter, AActor* DamageCauser);

    virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
    
    void AddStartupGameplayAbilities();
    
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
#pragma endregion

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    //
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void PostInitializeComponents() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    // Track Changing
    UPROPERTY(BlueprintReadWrite)
    bool bShouldChangeTracks = true;
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite)        // The actor passed in from the editor
    ASplineTrack* WorldSplineRef;

    // FUNCTIONS
    UFUNCTION(BlueprintCallable)
    void ToggleTrainStop(bool bExtendLeft = false, bool bExtendRight = false);
    
    // Updating the Objective Message
    UFUNCTION(BlueprintCallable)
    void UpdateObjectiveText(FString NewText = "");
    
    UBoxComponent* GetEngineDetectionComponent() const;
    ATrainCarriage* GetLastCarriage();

    void DisableMovement();
    void EnableMovement();

    bool GetTrainMoving() const;
    void ActivateLeverControls();

    // Train Speed Modification
    // Modify Speed Modifier
    UFUNCTION(BlueprintCallable)
    void ChangeTrainSpeedModifier(float NewModifier = 1.0f);
    void ResetTrainSpeed();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void MovePlatforms(bool bExtendLeft, bool bExtendRight);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void MoveLeftPlatform(bool bExtending);
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void MoveRightPlatform(bool bExtending);

private:    
    // VARIABLES
    // Obtaining the spline for the train to follow
    UPROPERTY(EditInstanceOnly)
    int TimeToComplete = 30;
    float SplineLength;
    
    float TimeSinceStart;
    bool bStartedMoving = false;
    bool bTrainMoving = false;
    UPROPERTY(EditAnywhere)
    bool bMoveOnStart = true;

    // Train Speed Modification
    FTimeline TrainSpeedModificationTimeline;
    UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
    UCurveFloat* MovementCurve;
    const float DefaultTrainSpeedModifier = 1.0f;
    float CurrentModifier = 1.0f;
    float TargetModifier = 1.0f;
    float TrainSpeedModifier = 1.0f;
    
    // Extra controls that have additional functionality
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ATrainControlls> TrainControlsClass;
    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    ATrainControlls* TrainControls;
    bool bCanActivateTrainControls = true;

    //
    UPROPERTY(EditDefaultsOnly)
    UStaticMesh* EngineMesh = nullptr;

    // CARRIAGE HANDLING SECTION
    UPROPERTY(EditDefaultsOnly)
    TArray<UStaticMesh*> StaticMeshRefs;

    // Carriage Class
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ATrainCarriage> TrainCarriageClass = nullptr;
    
    TArray<ATrainCarriage*> CarriageRefs;
    UPROPERTY(EditInstanceOnly)
    int CarriageCount = 0;
    const int DistanceFromFront = 2000;
    const int DistanceBetweenCarriages = 1400;

    float EngineStart = 0;

    // Objective Text Integration
    UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    FString CurrentObjectiveMessage = TEXT("");
    // Objective Movement Prevention
    bool bObjectiveLocked = false;
    
    // FUNCTIONS
    // Collision
    virtual void BeginCarOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
        ) override;
    virtual void EndCarOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
        ) override;

    // Train Speed Modification
    UFUNCTION()
    void ProcessTrainSpeedTimeline(float TimelineProgress);
};
