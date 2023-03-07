// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "TrainCarriage.h"
#include "TrainEngine.generated.h"

// Enums
UENUM(BlueprintType)
enum class TrainSpeed : uint8
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

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Custom section containing everything added beyond the default Unreal code
    
    // Track Changing
    UPROPERTY(BlueprintReadWrite)
    bool ShouldChangeTracks = true;
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite)        // The actor passed in from the editor
    AActor* TrackActorRef;

    // Functions
    UFUNCTION(BlueprintCallable)
    void ToggleTrainStop();

    UFUNCTION(BlueprintCallable)
    void SetTrainSpeed(TrainSpeed NewSpeed);
    // UFUNCTION(BlueprintCallable)
    // bool ChangeTrack(AActor* NewTrack);
    //
    // UFUNCTION(BlueprintCallable)
    // void GetSplineReferences(TArray<class ASplineTrack*>& Array);
    //
    // UFUNCTION(BlueprintCallable)
    // void BPOverrideTrack(AActor* TrackOverride);
    // bool GetTrackOverrideState();

protected:


private:
    // Structs
    struct FSplineTraversalParameters
    {
        float TimeToTraverse;
        float LengthToTraverse;
        float TimeToSwap;
        float Ratio;
    };
    
    // Variables
    // Obtaining the spline for the train to follow
    // UPROPERTY(EditInstanceOnly, BlueprintReadWrite)        // The actor passed in from the editor
    // AActor* TrackActorRef;
    USplineComponent* TrackSplineRef;
    float SplineLength;
    UPROPERTY(EditInstanceOnly)
    int TimeToComplete = 30;
    float TimeSinceStart;
    bool bHasStartedMoving;
    bool firstRun = false;
    bool isTrainMoving = true;
    float TrainSpeedModifier = 1.0f;

    // Creating the timer handle used to start the movement after a short delay
    FTimerHandle StartMoveTimerHandle;

    // Used to keep track for the lerp
    float LerpTimer;
    
    UPROPERTY(EditInstanceOnly)
    int StartDelayTime;

    float EngineStart;

    // Keeping track of the entire track (pun intended)
    TArray<class ASplineTrack*> CompleteTrackRefs;

    // Checking if the track has been overridden
    bool isTrackOverridden = false;

    // Complete Track Information
    float CompleteSplineLength;
    TArray<FSplineTraversalParameters> SplineTravelParameters;

    // Tracking current Spline
    int CurrentSplineIndex;
    float CurrentSplineTimeToTraverse;

    // Holds the Shapes used to show the engine
    USceneComponent* SceneRoot;

    UPROPERTY(EditInstanceOnly)
    UStaticMeshComponent* BoxComp;

    UPROPERTY(EditInstanceOnly)
    UArrowComponent* ArrowComp;

    // Track Changing
    // UPROPERTY(BlueprintReadWrite)
    // bool ShouldChangeTracks = false;

    // Functions
};
