// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2022 Media Design School
//
// File Name   : Boss.h
// Description : Main code for the boss
// Author      : Borderline Studios - Toni Natta
// Mail        : toni.natta@mds.ac.nz

#include "Boss.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "QRGameplayAbility.h"
#include "Components/CapsuleComponent.h"

ABoss::ABoss()
{
	// Set Up the state machines & their states
	StateMachineSetUps();
	
}

//ABoss::~ABoss()
//{

//	if (GetWorldTimerManager().IsTimerActive(TrackerAttackHandle))
//	{
//		GetWorld()->GetTimerManager().ClearTimer(TrackerAttackHandle);
//	}
//	
//	if (GetWorldTimerManager().IsTimerActive(IdleHandle))
//	{
//		GetWorld()->GetTimerManager().ClearTimer(IdleHandle);
//	}
//
//	if (GetWorldTimerManager().IsTimerActive(IdleSeq3Handle))
//	{
//		GetWorld()->GetTimerManager().ClearTimer(IdleSeq3Handle);
//	}
//
//	if (GetWorldTimerManager().IsTimerActive(ObjDropResetHandle))
//	{
//		GetWorld()->GetTimerManager().ClearTimer(ObjDropResetHandle);
//	}
//	
//	Tracker = nullptr; // Tracker obj
//	Laser = nullptr; // Laser left Obj
//	DoubleLaserL = nullptr; // Laser left Obj double lasers
//	DoubleLaserR = nullptr; // Laser left Obj double lasers
//	
//	
//}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Reference failed to load... Boss.cpp"));
		
		PlayerFeetZ = PlayerRef->GetActorLocation().Z - (PlayerRef->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.5);
	}

	// Load the laser
	LaserRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Effects/Enemies/Boss/Laser/BP_Laser.BP_Laser_C'")));
	LaserClassRef = LaserRef.LoadSynchronous();

	// Load the tracker
	TrackerRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Boss/Tracker.Tracker_C'")));
	TrackerClassRef = TrackerRef.LoadSynchronous();

	// Load the object to be spawned
	ObjDroppedRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Boss/ObjectDropped.ObjectDropped_C'")));
	ObjDroppedClassRef = ObjDroppedRef.LoadSynchronous();

	
	// Load the object to be checked
	GenShieldRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Interactables/BossRoom/BP_ShieldGenerator.BP_ShieldGenerator_C'")));
	GenShieldClassRef = GenShieldRef.LoadSynchronous();

	// Load the object to be checked
	BlockingBoxRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Boss/BP_BlockingBox.BP_BlockingBox_C'")));
	BlockingBoxClassRef = BlockingBoxRef.LoadSynchronous();

	// Load the object to be checked
	DeathEffectRef = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Production/Enemies/Boss/BP_BossDeath.BP_BossDeath_C'")));
	DeathEffectClassRef = DeathEffectRef.LoadSynchronous();

    LeftSocket = TEXT("L_LaserSocket");
    RightSocket = TEXT("R_LaserSocket");
}

void ABoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorld()->GetTimerManager().ClearTimer(TrackerAttackHandle);

	GetWorld()->GetTimerManager().ClearTimer(IdleHandle);
	
	GetWorld()->GetTimerManager().ClearTimer(IdleSeq3Handle);

	GetWorld()->GetTimerManager().ClearTimer(ObjDropResetHandle);
	
	GetWorld()->GetTimerManager().ClearTimer(EndGameHandle);
	
	Tracker = nullptr; // Tracker obj
	Laser = nullptr; // Laser left Obj
	DoubleLaserL = nullptr; // Laser left Obj double lasers
	DoubleLaserR = nullptr; // Laser left Obj double lasers
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// INDEX IS UPDATED WHEN DAMAGED
	if (FunctionPtrs.Num() > 0 && StateMachines.Num() > 0)
	{
		(this->*(FunctionPtrs[StateMachines[currentStateMachineIndex].CurrentState->index]))(DeltaTime);
	}
	//UE_LOG(LogTemp, Warning, TEXT("index %d"), StateMachines[currentStateMachineIndex].CurrentState->index);

}

void ABoss::StateMachineSetUps()
{
#pragma region CreateVariables	
	// STATES
	StateMachine::FState* StartingBootUpState = new StateMachine::FState; // Sequence 1 - Starting animation 
	StateMachine::FState* IdleSeq1State = new StateMachine::FState; // Sequence 1 - Idle
	StateMachine::FState* LaserSeq1State = new StateMachine::FState; // Sequence 1 - Laser 

	StateMachine::FState* ShieldUpSeq2 = new StateMachine::FState; // Sequence 2 - Generated shield up
	StateMachine::FState* ParkourModeState = new StateMachine::FState; // Sequence 2 - Parkour
	StateMachine::FState* IdleSeq2State = new StateMachine::FState; // Sequence 2 - Idle
	StateMachine::FState* ObjDropSeq2State = new StateMachine::FState; // Sequence 2 - Fist Attack 
	StateMachine::FState* ObjDropResetSeq2State = new StateMachine::FState; // Sequence 2 - Fist Attack Reset

	StateMachine::FState* PersonalShieldSeq3 = new StateMachine::FState; // Sequence 3 - Personal shield up
	StateMachine::FState* IdleSeq3State = new StateMachine::FState; // Sequence 3 - Idle
	StateMachine::FState* DoubleLaserSeq3State = new StateMachine::FState; // Sequence 3 - Double Laser
	StateMachine::FState* ObjDropSeq3State = new StateMachine::FState; // Sequence 3 - ObjDrop Attack 
	StateMachine::FState* ObjDropResetSeq3State = new StateMachine::FState; // Sequence 3 - Fist Attack Reset

	// STATE MACHINES
	StateMachine::FStateMachine Sequence1; // Sequence 1 state machine - performs boss' 1st sequence
	StateMachine::FStateMachine Sequence2; // Sequence 2 state machine - performs boss' 2nd sequence
	StateMachine::FStateMachine Sequence3; // Sequence 3 state machine - performs boss' 3rd sequence
#pragma endregion 

#pragma region SetStates
	
	// STATES - Sequence 1
	// Starting boot up - add transition(s) to array
	StartingBootUpState->index = 6; // boot up
	StartingBootUpState->NextStates.Add(IdleSeq1State); // Transition to idle

	// Idle seq 1 - add transition(s) to array 
	IdleSeq1State->index = 0; // idle
	IdleSeq1State->NextStates.Add(LaserSeq1State);
	
	// Laser seq 1 - add transition(s) to array 
	LaserSeq1State->index = 1; // laser
	LaserSeq1State->NextStates.Add(IdleSeq1State);

	
	// STATES - Sequence 2
	// shield up seq 2 - add transition(s) to array
	ShieldUpSeq2->index = 7; // gen shield up
	ShieldUpSeq2->NextStates.Add(ParkourModeState);
	
	// parkour mode seq 2 - add transition(s) to array 
	ParkourModeState->index = 5; // parkour
	ParkourModeState->NextStates.Add(IdleSeq2State);

	// Idle seq 2 - add transition(s) to array 
	IdleSeq2State->index = 0; // idle
	IdleSeq2State->NextStates.Add(ObjDropSeq2State);
	
	// ObjDrop seq 2 - add transition(s) to array 
	ObjDropSeq2State->index = 3; // obj drop attack
	ObjDropSeq2State->NextStates.Add(ObjDropResetSeq2State);

	// ObjDrop reset seq 2 - add transition(s) to array 
	ObjDropResetSeq2State->index = 4; // obj drop attack reset
	ObjDropResetSeq2State->NextStates.Add(IdleSeq2State);

	
	// STATES - Sequence 3
	// personal shield up seq 3 - add transition(s) to array 
	PersonalShieldSeq3->index = 8; // personal shield up
	PersonalShieldSeq3->NextStates.Add(IdleSeq3State);

	// Idle seq 3 - add transition(s) to array 
	IdleSeq3State->index = 9; // idle seq 3
	IdleSeq3State->NextStates.Add(ObjDropSeq3State);
	IdleSeq3State->NextStates.Add(DoubleLaserSeq3State);
	
	// Laser seq 3 - add transition(s) to array 
	DoubleLaserSeq3State->index = 2; // double laser
	DoubleLaserSeq3State->NextStates.Add(IdleSeq3State);

	// Fist attack seq 3 - add transition(s) to array 
	ObjDropSeq3State->index = 3; // obj drop attack
	ObjDropSeq3State->NextStates.Add(ObjDropResetSeq3State);

	// Fist reset seq 3 - add transition(s) to array 
	ObjDropResetSeq3State->index = 4; // obj drop attack reset;
	ObjDropResetSeq3State->NextStates.Add(IdleSeq3State);
	
#pragma endregion	

#pragma region SetStateMachines
	
	// STATESMACHINES
	// Sequence 1 - Set states
	Sequence1.States.Add(StartingBootUpState);
	Sequence1.States.Add(IdleSeq1State);
	Sequence1.States.Add(LaserSeq1State);
	Sequence1.CurrentState = StartingBootUpState;

	// Sequence 2 - Set States
	Sequence2.States.Add(ShieldUpSeq2);
	Sequence2.States.Add(ParkourModeState);
	Sequence2.States.Add(IdleSeq2State);
	Sequence2.States.Add(ObjDropSeq2State);
	Sequence2.States.Add(ObjDropResetSeq2State);
	Sequence2.CurrentState = ShieldUpSeq2;

	// Sequence 3 - Set States
	Sequence3.States.Add(PersonalShieldSeq3);
	Sequence3.States.Add(IdleSeq3State);
	Sequence3.States.Add(DoubleLaserSeq3State);
	Sequence3.States.Add(ObjDropSeq3State);
	Sequence3.States.Add(ObjDropResetSeq3State);
	Sequence3.CurrentState = PersonalShieldSeq3;

	StateMachines.Add(Sequence1);
	StateMachines.Add(Sequence2);
	StateMachines.Add(Sequence3);
	
#pragma endregion

	FunctionPtrs.Add(&ABoss::Idle);
	FunctionPtrs.Add(&ABoss::Lasers);
	FunctionPtrs.Add(&ABoss::DoubleLaser);
	FunctionPtrs.Add(&ABoss::ObjDropAttack);
	FunctionPtrs.Add(&ABoss::ObjDropAttackReset);
	FunctionPtrs.Add(&ABoss::Parkour);
	FunctionPtrs.Add(&ABoss::StartBootUp);
	FunctionPtrs.Add(&ABoss::GenShieldUp);
	FunctionPtrs.Add(&ABoss::PersonalShieldUp);
	FunctionPtrs.Add(&ABoss::IdleSeq3);
	
	
}

void ABoss::Idle(float DeltaTime)
{
	if (!bIdleTimerStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle"));

		if (!GetWorldTimerManager().IsTimerActive(IdleHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(IdleHandle, FTimerDelegate::CreateLambda([&]
			{
				UE_LOG(LogTemp, Warning, TEXT("idle timer done"));
				bIdleTimerStarted = false;
				StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
				GetWorld()->GetTimerManager().ClearTimer(IdleHandle);
			}), FMath::RandRange(1.5f, 3.0f), false);
		}
		bIdleTimerStarted = true;
	}

	if (CheckSwitchSequence())
	{
		GetWorld()->GetTimerManager().ClearTimer(IdleHandle);
		bIdleTimerStarted = false;
	}
}

/**
 * @brief Laser attack for the boss
 */
void ABoss::Lasers(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Laser pew pew"));

	if (!bLaserStarted)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName(TEXT("LaserLeft")));
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABoss::LasersAnimNotify);
		bLaserStarted = true;
	}

	if (bLaserSpawned && bLeftLaser)
	{
		LeftSocketLoc = GetMesh()->GetSocketLocation(LeftSocket);
		LeftSocketRot = GetMesh()->GetSocketRotation(LeftSocket);
	
		Laser->SetActorLocation(LeftSocketLoc);
		Laser->SetActorRotation(LeftSocketRot);
	}
	else if (bLaserSpawned && !bLeftLaser)
	{
		RightSocketLoc = GetMesh()->GetSocketLocation(RightSocket);
		RightSocketRot = GetMesh()->GetSocketRotation(RightSocket);
		
		Laser->SetActorLocation(RightSocketLoc);
		Laser->SetActorRotation(RightSocketRot);
	}
}

/**
 * @brief Resets laser to original position & rotation
 */
 void ABoss::DoubleLaser(float DeltaTime)
{
	
	if (!bDoubleLaserStarted)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName(TEXT("DoubleLaser")));
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABoss::DoubleLasersAnimNotify);
		bDoubleLaserStarted = true;
	}

	if (bDoubleLaserSpawned)
	{
		LeftSocketLoc = GetMesh()->GetSocketLocation(LeftSocket);
		LeftSocketRot = GetMesh()->GetSocketRotation(LeftSocket);
		
		DoubleLaserL->SetActorLocation(LeftSocketLoc);
		DoubleLaserL->SetActorRotation(LeftSocketRot);

		RightSocketLoc = GetMesh()->GetSocketLocation(RightSocket);
		RightSocketRot = GetMesh()->GetSocketRotation(RightSocket);
		
		DoubleLaserR->SetActorLocation(RightSocketLoc);
		DoubleLaserR->SetActorRotation(RightSocketRot);
	}
}

/**
 * @brief Fist attack for boss
 */
void ABoss::ObjDropAttack(float DeltaTime)
{
	PlayerFeetZ = PlayerRef->GetActorLocation().Z - (PlayerRef->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 1.5);
	FVector PlayerFeetLoc = FVector(PlayerRef->GetActorLocation().X, PlayerRef->GetActorLocation().Y, PlayerFeetZ);
	
	// If tracker hasnt spawned spawn it 
	if (!bTrackerSpawned)
	{
		Tracker = GetWorld()->SpawnActor<AActor>(TrackerClassRef, PlayerFeetLoc, FRotator::ZeroRotator);
		bTrackerSpawned = true;
	}

	if (Tracker != nullptr)
	{
		if (!GetWorldTimerManager().IsTimerActive(TrackerAttackHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(TrackerAttackHandle, FTimerDelegate::CreateLambda([&] { bTrackerAttackDone = true; UE_LOG(LogTemp, Warning, TEXT("timer done"));}), MaxTrackerTime, false);
			UE_LOG(LogTemp, Warning, TEXT("timer set")); // Condition
		}

		if (CheckSwitchSequence() || bRequestEndGame)
		{
			Tracker->Destroy();
			GetWorld()->GetTimerManager().ClearTimer(TrackerAttackHandle);
			bTrackerAttackDone = false;
			bTrackerSpawned = false;
			bObjSpawned = false;
			return;
		}
		
		
		if (!bTrackerAttackDone)
		{
			Tracker->SetActorLocation(FMath::Lerp(Tracker->GetActorLocation(), PlayerFeetLoc, DeltaTime * TrackerSpeed));
		}
		else
		{
			if (!bObjSpawned)
			{
				GetWorld()->GetTimerManager().ClearTimer(TrackerAttackHandle);

				GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName(TEXT("ObjectSpawn")));
				AActor* HeavyObject = GetWorld()->SpawnActor<AActor>(ObjDroppedClassRef, FVector(Tracker->GetActorLocation().X, Tracker->GetActorLocation().Y, Tracker->GetActorLocation().Z + ObjectSpawnHeight), FRotator::ZeroRotator);

				if (HeavyObject)
				{
					bObjSpawned = true;
					StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
					UE_LOG(LogTemp, Warning, TEXT("Obj spawned")); // Condition
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Obj no spawn")); // Condition
				}
			}
		}
	}
}

/**
 * @brief Resets fist to original position & rotation
 */
void ABoss::ObjDropAttackReset(float DeltaTime)
{
	if (!bObjDropResetStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("attac reset :)"));

		if (!GetWorldTimerManager().IsTimerActive(ObjDropResetHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(ObjDropResetHandle, FTimerDelegate::CreateLambda([&]
			{

				bTrackerSpawned = false;
				bTrackerAttackDone = false;
				bObjSpawned = false;

				Tracker = nullptr;
				
				UE_LOG(LogTemp, Warning, TEXT("objdropreset timer done"));
				bObjDropResetStarted = false;
				StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
				GetWorld()->GetTimerManager().ClearTimer(ObjDropResetHandle);
			}), 2.5f, false);
		}
		bObjDropResetStarted = true;
	}

	if (CheckSwitchSequence() || bRequestEndGame)
	{
		bTrackerSpawned = false;
		bTrackerAttackDone = false;
		bObjSpawned = false;
		Tracker = nullptr;
		bObjDropResetStarted = false;
		GetWorld()->GetTimerManager().ClearTimer(ObjDropResetHandle);
	}
	
}

/**
 * @brief Parkour segment
 */
void ABoss::Parkour(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Parkour"));

	//GetMesh()->GetAnimInstance()->Montage_JumpToSection("GenShieldIdle");

	if (!bParkourUp)
	{
		ActivatePlatforms();
		bParkourUp = true;
	}
	

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GenShieldClassRef, FoundGens);

	UE_LOG(LogTemp, Warning, TEXT("%d"), FoundGens.Num());
	
	if (FoundGens.Num() == 0 && !bGenShieldAnim)
	{
		// no more gens
		UE_LOG(LogTemp, Warning, TEXT("No more gens switching"));
		DeactivatePlatforms();
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName(TEXT("GenShieldDown")));
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABoss::StartUpAnimsNotify);
		bGenShieldAnim = true;
		//StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
	}

	CheckSwitchSequence();
	
}

void ABoss::StartBootUp(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Boot up"));

	if (!bBootUpStarted)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName(TEXT("BootUp")));
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABoss::StartUpAnimsNotify);
		bBootUpStarted = true;
	}
}

void ABoss::GenShieldUp(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("GenShield up"));
	
	if (!bGenShieldUp)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName(TEXT("GenShieldUp")));
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABoss::StartUpAnimsNotify);
		bGenShieldUp = true;
	}

	//StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
}

void ABoss::PersonalShieldUp(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("personal shield up"));
	
	if (!bPersonalShieldUp)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName(TEXT("PersonalShieldUp")));
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABoss::StartUpAnimsNotify);
		bPersonalShieldUp = true;
	}
}

void ABoss::IdleSeq3(float DeltaTime)
{
	if (!bIdleSeq3TimerStarted)
	{
		UE_LOG(LogTemp, Warning, TEXT("Idle seq 3"));
		if (!GetWorldTimerManager().IsTimerActive(IdleSeq3Handle))
		{
			GetWorld()->GetTimerManager().SetTimer(IdleSeq3Handle, FTimerDelegate::CreateLambda([&]
			{
				UE_LOG(LogTemp, Warning, TEXT("idle seq 3 timer done"));
				bIdleSeq3TimerStarted = false;
				StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[FMath::RandRange(0,1)];
				GetWorld()->GetTimerManager().ClearTimer(IdleSeq3Handle);
			}), FMath::RandRange(2.5f, 4.0f), false);

			bIdleSeq3TimerStarted = true;
		}
	}

	if (CheckSwitchSequence() || bRequestEndGame)
	{
		GetWorld()->GetTimerManager().ClearTimer(IdleSeq3Handle);
		bIdleSeq3TimerStarted = false;
	}
}

void ABoss::NewSequenceEffect(int NewSequenceNum)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);
	if (ASC)
	{
		if (currentStateMachineIndex == 0)
		{
			ASC->ApplyGameplayEffectToTarget(PassiveGameplayEffects[0].GetDefaultObject(), ASC);
		}
		if (currentStateMachineIndex == 1)
		{
			// Get rid of blockers
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), BlockingBoxClassRef, FoundBlockers);

			UE_LOG(LogTemp, Warning, TEXT(" fences found: %d"), FoundBlockers.Num());

			for (int i = 0; i < FoundBlockers.Num(); i++)
			{
				FoundBlockers[i]->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 1000.0f));
			}
			
			ASC->ApplyGameplayEffectToTarget(PassiveGameplayEffects[1].GetDefaultObject(), ASC);
			Seq2ShieldSetup();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Phase2StartSFX, PlayerRef->GetActorLocation());
		}
		if (currentStateMachineIndex == 2)
		{
			// Get rid of blockers
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), BlockingBoxClassRef, FoundBlockers);

			UE_LOG(LogTemp, Warning, TEXT("%d"), FoundBlockers.Num());

			for (int i = 0; i < FoundBlockers.Num(); i++)
			{
				FoundBlockers[i]->SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 1000.0f));
			}
			
			ASC->ApplyGameplayEffectToTarget(PassiveGameplayEffects[2].GetDefaultObject(), ASC);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Phase3StartSFX, PlayerRef->GetActorLocation());
		}
	}

}

bool ABoss::CheckSwitchSequence()
{
	if (bRequestSwitch)
	{
		currentStateMachineIndex++;
		NewSequenceEffect(currentStateMachineIndex);
		bRequestSwitch = false;
		return true;
	}

	return false;
}


void ABoss::PostHitProcess()
{
	// get ability system component
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);

	// bool to check if it was found & the value the health equals
	bool bFound;
	float Value = ASC->GetGameplayAttributeValue(UQRAttributeSet::GetHealthAttribute(), bFound);

	if (Value <= 0 && currentStateMachineIndex != 2)
	{
		bRequestSwitch = true;
	}
	else if (Value <= 0 && currentStateMachineIndex == 2)
	{
		bRequestEndGame = true;

		// if in lasers delete them
		if (DoubleLaserL && DoubleLaserR)
		{
			bDoubleLaserSpawned = false;
			DoubleLaserL->Destroy();
			DoubleLaserR->Destroy();
			DoubleLaserL = nullptr;
			DoubleLaserR = nullptr;
		}

		UE_LOG(LogTemp, Warning, TEXT("END GAME")); // Condition
		
		// spawn boss death vfx
		this->SetActorHiddenInGame(true);
		FVector location =  this->GetActorLocation();
		AActor* deathEffect = Cast<AActor>(GetWorld()->SpawnActor(DeathEffectClassRef, &location));
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathEffect, this->GetActorLocation(), this->GetActorRotation(), FVector(1,1,1), true);
		
		GetWorld()->GetTimerManager().SetTimer(EndGameHandle, FTimerDelegate::CreateLambda([&]
			{
				GetWorld()->GetTimerManager().ClearTimer(EndGameHandle);
				EndGame();
			}), EndGameTimer, false);
		
		
	}
}

void ABoss::LasersAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName(TEXT("SpawnLaserLeft")))
	{
		if (!bLaserSpawned)
		{
			LeftSocketLoc = GetMesh()->GetSocketLocation(LeftSocket);
			LeftSocketRot = GetMesh()->GetSocketRotation(LeftSocket);
			
			Laser = GetWorld()->SpawnActor<AActor>(LaserSubclassRef, LeftSocketLoc , LeftSocketRot);
			bLeftLaser = true;
				
			if (Laser)
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser left spawned")); // Condition
				Laser->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,  false), LeftSocket);
				bLaserSpawned = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser lefty no spawned")); // Condition
			}
		}
	}
	else if (NotifyName == FName(TEXT("SpawnLaserRight")))
	{
		if (!bLaserSpawned)
		{
			RightSocketLoc = GetMesh()->GetSocketLocation(RightSocket);
			RightSocketRot = GetMesh()->GetSocketRotation(RightSocket);
			
			Laser = GetWorld()->SpawnActor<AActor>(LaserSubclassRef, RightSocketLoc , RightSocketRot);

			if (Laser)
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser right spawned")); // Condition
				Laser->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,  false), RightSocket);
				bLaserSpawned = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser right no spawned")); // Condition
			}
		}
	}
	else if (NotifyName == FName(TEXT("DeleteLaser")))
	{
		if (Laser)
		{
			bLaserSpawned = false;
			bLeftLaser = false;
			UE_LOG(LogTemp, Warning, TEXT("Laser bye bye")); // Condition
			Laser->Destroy();
			Laser = nullptr;
			CheckSwitchSequence();
		}
	}
	else if (NotifyName == FName(TEXT("LasersDone")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Switching to next state LASERSDONE"));
		bLaserStarted = false;
		StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
	}
}

void ABoss::DoubleLasersAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	//DoubleLasersDone DeleteDoubleLaser SpawnDoubleLaser

	if (NotifyName == FName(TEXT("SpawnDoubleLaser")))
	{
		if (!bLaserSpawned)
		{
			LeftSocketLoc = GetMesh()->GetSocketLocation(LeftSocket);
			LeftSocketRot = GetMesh()->GetSocketRotation(LeftSocket);

			RightSocketLoc = GetMesh()->GetSocketLocation(RightSocket);
			RightSocketRot = GetMesh()->GetSocketRotation(RightSocket);
			
			DoubleLaserL = GetWorld()->SpawnActor<AActor>(LaserSubclassRef, LeftSocketLoc , LeftSocketRot);
			DoubleLaserR = GetWorld()->SpawnActor<AActor>(LaserSubclassRef, RightSocketLoc , RightSocketRot);
				
			if (DoubleLaserL && DoubleLaserR)
			{
				UE_LOG(LogTemp, Warning, TEXT("Laser double spawned")); // Condition
				DoubleLaserL->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,  false), LeftSocket);
				DoubleLaserR->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,EAttachmentRule::KeepRelative,EAttachmentRule::KeepWorld,  false), RightSocket);
				bDoubleLaserSpawned = true;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("double Laser no spawned")); // Condition
			}
		}
	}
	else if (NotifyName == FName(TEXT("DeleteDoubleLaser")))
	{
		if (DoubleLaserL && DoubleLaserR)
		{
			bDoubleLaserSpawned = false;
			UE_LOG(LogTemp, Warning, TEXT("double Laser bye bye")); // Condition
			DoubleLaserL->Destroy();
			DoubleLaserR->Destroy();
			DoubleLaserL = nullptr;
			DoubleLaserR = nullptr;
			CheckSwitchSequence();
		}
	}
	else if (NotifyName == FName(TEXT("DoubleLasersDone")))
	{
		UE_LOG(LogTemp, Warning, TEXT("double lasers done switched states"));
		bDoubleLaserStarted = false;
		StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
	}
}

void ABoss::StartUpAnimsNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == FName(TEXT("AnimEnding")))
	{
		UE_LOG(LogTemp, Warning, TEXT("anim end"));
		bBootUpStarted = false;
		bGenShieldUp = false;
		bPersonalShieldUp = false;
		bGenShieldAnim = false;
		bParkourUp = false;
		StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
	}
	else if (NotifyName == FName(TEXT("GenShieldUp")))
	{
		StateMachines[currentStateMachineIndex].CurrentState = StateMachines[currentStateMachineIndex].CurrentState->NextStates[0];
	}
}
