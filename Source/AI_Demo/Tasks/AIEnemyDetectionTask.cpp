//FilloutyourcopyrightnoticeintheDescriptionpageofProjectSettings.


#include"Tasks/AIEnemyDetectionTask.h"
#include"TimerManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "AI/Controllers/AI_Demo_AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"


UAIEnemyDetectionTask* UAIEnemyDetectionTask::StartAIEnemyDetectionTask(
	UObject*InWorldContextObject,
	float InTickRatePerSecond,
	UAIPerceptionComponent* InPerceptionComponent,
	const FAIEnemyDetectionTaskPayload& InPatrolStateRules)
{
	UAIEnemyDetectionTask* AsyncTask = NewObject<UAIEnemyDetectionTask>();
	AsyncTask->WorldContextObject = InWorldContextObject;
	AsyncTask->TickRatePerSecondInternal = InTickRatePerSecond;
	AsyncTask->PerceptionComponentInternal = InPerceptionComponent;
	AsyncTask->PatrolStateRulesInternal = InPatrolStateRules;
	AsyncTask->RegisterWithGameInstance(InWorldContextObject);
	return AsyncTask;
}

void UAIEnemyDetectionTask::Activate()
{
	Super::Activate();
	
	if (!TryGetWorld())
		return;
	
	//Bind Perception Events
	if (!TryGetPerceptionComponent())
		return;
	
	// Get AI controller from perception component
	if (!TryGetAIController())
		return;
	
	// Bind perception update events
	BindPerceptionEvents();


	ValidateTickRate();

	OnTaskStarted.Broadcast(this);

}

void UAIEnemyDetectionTask::EndTask()
{
	StopTick();
	SetReadyToDestroy();
}

bool UAIEnemyDetectionTask::TryGetWorld()
{
	if (!WorldContextObject.IsValid())
	{
		//Cannot activate without a valid world context
		SetReadyToDestroy();
		return false;
	}

	WorldRefInternal = WorldContextObject->GetWorld();
	if (!WorldRefInternal)
	{
		//Cannot activate without a valid world
		SetReadyToDestroy();
		return false;
	}
	return true;
}


// Perception

bool UAIEnemyDetectionTask::TryGetPerceptionComponent()
{
	if (!PerceptionComponentInternal)
	{
		//Cannot activate without a valid world
		SetReadyToDestroy();
		return false;
	}
	return true;
}

void UAIEnemyDetectionTask::BindPerceptionEvents()
{
	PerceptionComponentInternal->OnTargetPerceptionUpdated.AddDynamic(this, &UAIEnemyDetectionTask::OnTargetUpdated);

}


// Tick

void UAIEnemyDetectionTask::AsyncTick()
{
	const float Delta = 1.f / TickRatePerSecondInternal;

	if (IsEnemyDetected())
	{
		if (HasStartedWindDown)
		{
			AbortWindDown();
		}

		ElapsedTime += Delta;

		if (CurrentWindUpTime > 0.f)
		{
			ElapsedTime = FMath::Clamp(ElapsedTime, 0.f, CurrentWindUpTime);
		}

		if (CanForwardOneState() && CurrentWindUpTime > 0.f && FMath::IsNearlyEqual(ElapsedTime, CurrentWindUpTime))
		{
			MoveToNextState();
		}
	}
	else
	{
		if (ElapsedTime > 0.f)
		{
			ElapsedTime -= Delta;
			ElapsedTime = FMath::Max(0.f, ElapsedTime);
		}

		if (FMath::IsNearlyZero(ElapsedTime))
		{
			if (!HasStartedWindDown && !IsPatrolling())
			{
				StartWindDown();
				// We should stop the tick while waiting for the wind down timer
				StopTick();
			}
		}
	}

	UpdateAlpha();
	OnAlphaUpdate.Broadcast(Alpha);

	TryToStopTick();
}

void UAIEnemyDetectionTask::ValidateTickRate()
{
	if (TickRatePerSecondInternal <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAIEnemyDetectionTask: TickRatePerSecond must be greater than 0.0f. Defaulting to 10.0f."));
		TickRatePerSecondInternal = 10.0f;
	}
}

void UAIEnemyDetectionTask::StopTick()
{	
	if (!bIsTickEnabled)
		return;

	if (WorldRefInternal)
	{
		WorldRefInternal->GetTimerManager().ClearTimer(TickTimerHandle);
		bIsTickEnabled = false;
	}
}

void UAIEnemyDetectionTask::StartTick()
{
	if (bIsTickEnabled)
		return;

	bIsTickEnabled = true;
	float TickInterval = 1.0f / TickRatePerSecondInternal;
	WorldRefInternal->GetTimerManager().SetTimer(TickTimerHandle, this, &UAIEnemyDetectionTask::AsyncTick, TickInterval, true, 0.0f);
}

bool UAIEnemyDetectionTask::TryGetAIController()
{
	// Perception component was previously validated and this is sync so no need to check

	if (auto Owner = PerceptionComponentInternal->GetOwner())
		if (AAI_Demo_AIControllerBase* CastedOwner = Cast< AAI_Demo_AIControllerBase>(Owner))
			AIControllerInternal = CastedOwner;

	return AIControllerInternal != nullptr;
}


// Events

void UAIEnemyDetectionTask::OnTargetUpdated(AActor* InActor, FAIStimulus InStimulus)
{
	if (InStimulus.Type != UAISense::GetSenseID<UAISense_Sight>())
		return;

	bool bIsDetected = InStimulus.WasSuccessfullySensed();

	if (bIsDetected)
		ActiveActors.Add(InActor);
	else
		ActiveActors.Remove(InActor);

	CurrentWindDownTime = GetCurrentWindDownTime();
	CurrentWindUpTime = GetCurrentWindUpTime();

	if (IsEnemyDetected())
	{
		StartTick();
	}
}


// Helpers

bool UAIEnemyDetectionTask::IsPatrolling()
{
	return PatrolStateRulesInternal.UpdateCurrentPatrolState(AIControllerInternal) == EAIPatrolState::Patrolling;
}

bool UAIEnemyDetectionTask::IsEnemyDetected()
{
	return ActiveActors.Num() > 0;
}

bool UAIEnemyDetectionTask::IsAlphaZeroed()
{
	return Alpha <= KINDA_SMALL_NUMBER;
}

void UAIEnemyDetectionTask::MoveToNextState()
{
	if (!CanForwardOneState())
		return;
	
	// Update Struct
	PatrolStateRulesInternal.UpdateCurrentPatrolState(AIControllerInternal);
	
	//Get Next Value
	EAIPatrolState NextState = GetNextState();

	// Update BB
	UpdateBlackBoardValue(NextState);

	//Broadcast Change
	OnPatrolStateUpdate.Broadcast(NextState);

	// Update to new Times
	CurrentWindUpTime = GetCurrentWindUpTime();
	CurrentWindDownTime = GetCurrentWindDownTime();

	// Reset Elapsed Time
	ElapsedTime = 0;
}

void UAIEnemyDetectionTask::MoveToPreviousState()
{
	if (!CanRegressOneState())
		return;

	// Update Struct
	PatrolStateRulesInternal.UpdateCurrentPatrolState(AIControllerInternal);
	//Get Next Value
	EAIPatrolState NextState = GetPreviousState();

	// Update BB
	UpdateBlackBoardValue(NextState);

	//Broadcast Change
	OnPatrolStateUpdate.Broadcast(NextState);

	// Update to new Times
	CurrentWindUpTime = GetCurrentWindUpTime();
	CurrentWindDownTime = GetCurrentWindDownTime();

	// Reset Elapsed Time
	ElapsedTime = 0;
}

float UAIEnemyDetectionTask::GetCurrentWindUpTime()
{
	EAIPatrolState CurrentPatrolState = PatrolStateRulesInternal.CurrentPatrolState;
	if (!PatrolStateRulesInternal.StateChangeData.Contains(CurrentPatrolState))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAIEnemyDetectionTask: CurrentPatrolState not found in StateChangeData. Returning 0.f for WindUpTime."));
		return 0.f;
	}

	return PatrolStateRulesInternal.StateChangeData[CurrentPatrolState].WindUpTime;
}

float UAIEnemyDetectionTask::GetCurrentWindDownTime()
{
	EAIPatrolState CurrentPatrolState = PatrolStateRulesInternal.CurrentPatrolState;
	if (!PatrolStateRulesInternal.StateChangeData.Contains(CurrentPatrolState))
	{
		UE_LOG(LogTemp, Error, TEXT("UAIEnemyDetectionTask: CurrentPatrolState not found in StateChangeData. Returning 0.f for WindDownTime."));
		return 0.f;
	}
	FPatrolStateWindTimes& CurrentData = PatrolStateRulesInternal.StateChangeData[CurrentPatrolState];

	// If its a custom state return the wind times of the entry with Key: FPatrolStateWindTimes::OverrideState
	if (CurrentData.IsCustomState)
	{
		if (!PatrolStateRulesInternal.StateChangeData.Contains(CurrentData.OverrideState))
		{
			UE_LOG(LogTemp, Error, TEXT("UAIEnemyDetectionTask: OverrideData State not found in StateChangeData. Returning 0.f for WindDownTime."));
			return 0.f;
		}

		return  PatrolStateRulesInternal.StateChangeData[CurrentData.OverrideState].WindDownTime;
	}

	return CurrentData.WindDownTime;
}

void UAIEnemyDetectionTask::StartWindDown()
{
	if (HasStartedWindDown)
		return;
	WorldRefInternal->GetTimerManager().SetTimer(WindDownTimerHandle, this, &UAIEnemyDetectionTask::OnWindDownFinished, GetCurrentWindDownTime(), false);
	HasStartedWindDown = true;
}

void UAIEnemyDetectionTask::AbortWindDown()
{
	if (!WindDownTimerHandle.IsValid())
		return;

	WorldRefInternal->GetTimerManager().ClearTimer(WindDownTimerHandle);
	HasStartedWindDown = false;
}

void UAIEnemyDetectionTask::OnWindDownFinished()
{
	HasStartedWindDown = false;
	ElapsedTime = 0;
	MoveToPreviousState();
	
	// After moving to the previous state, re-evaluate if the tick should be active.
	// The tick was stopped by StartWindDown(). If we are now in a state that requires further processing, restart it.
	// For example, if we are still not in the "Patrolling, no enemy, alpha zeroed" state, we should tick.
	// Otherwise, if we are in the final idle state, the tick should remain stopped.
	if (ActiveActors.Num() > 0 || !IsPatrolling() || !FMath::IsNearlyZero(Alpha))
	{
		StartTick();
	}
	else
	{
		// We have reached the fully idle state (Patrolling, no enemies, alpha zeroed),
		// so ensure the tick remains stopped. StopTick() was already called by StartWindDown().
		// This branch essentially confirms that it should stay stopped.
		StopTick();
	}
}

bool UAIEnemyDetectionTask::CanRegressOneState()
{
	EAIPatrolState CurrentPatrolState = PatrolStateRulesInternal.CurrentPatrolState;
	
	if (PatrolStateRulesInternal.StateChangeData[CurrentPatrolState].IsCustomState)
	{
		EAIPatrolState NewState = PatrolStateRulesInternal.StateChangeData[CurrentPatrolState].OverrideState;
		if (!PatrolStateRulesInternal.StateChangeData.Contains(NewState))
		{
			UE_LOG(LogTemp, Error, TEXT("UAIEnemyDetectionTask: The override state is not present in the detection rules, please add it. "));
			return 0.f;
		}
		CurrentPatrolState = NewState;
	}

	if (!PatrolStateRulesInternal.StateChangeData.Contains(CurrentPatrolState))
		return false;
	return (uint8)(PatrolStateRulesInternal.StateChangeData[CurrentPatrolState].TransitionRules & (uint8)EPatrolStateTransitionRule::Previous) != 0;
}

bool UAIEnemyDetectionTask::CanForwardOneState()
{
	EAIPatrolState CurrentPatrolState = PatrolStateRulesInternal.CurrentPatrolState;
	
	if (PatrolStateRulesInternal.StateChangeData[CurrentPatrolState].IsCustomState)
	{
		EAIPatrolState NewState = PatrolStateRulesInternal.StateChangeData[CurrentPatrolState].OverrideState;
		if (!PatrolStateRulesInternal.StateChangeData.Contains(NewState))
		{
			UE_LOG(LogTemp, Error, TEXT("UAIEnemyDetectionTask: The override state is not present in the detection rules, please add it. "));
			return 0.f;
		}
		CurrentPatrolState = NewState;
	}

	if (!PatrolStateRulesInternal.StateChangeData.Contains(CurrentPatrolState))
		return false;
	return (uint8)(PatrolStateRulesInternal.StateChangeData[CurrentPatrolState].TransitionRules & (uint8)EPatrolStateTransitionRule::Next) != 0;
}

EAIPatrolState UAIEnemyDetectionTask::GetNextState()
{
	TArray<EAIPatrolState> States;
	PatrolStateRulesInternal.StateChangeData.GetKeys(States);
	States = States.FilterByPredicate([&](EAIPatrolState State) { return !(PatrolStateRulesInternal.StateChangeData[State].IsCustomState); });
	States.Sort([](const EAIPatrolState& A, const EAIPatrolState& B) 
	{
		return (uint8)A < (uint8)B;
	});

	const int32 CurrentIndex = States.Find(PatrolStateRulesInternal.CurrentPatrolState);
	if (CurrentIndex != INDEX_NONE && CurrentIndex + 1 < States.Num())
	{
		return States[CurrentIndex + 1];
	}

	return PatrolStateRulesInternal.CurrentPatrolState;
}

EAIPatrolState UAIEnemyDetectionTask::GetPreviousState()
{
	TArray<EAIPatrolState> States;
	PatrolStateRulesInternal.StateChangeData.GetKeys(States);

	// Remove All Custom States from the Array. These are supposed to be manually set.
	States = States.FilterByPredicate([&](EAIPatrolState State) { return !(PatrolStateRulesInternal.StateChangeData[State].IsCustomState); });

	States.Sort([](const EAIPatrolState& A, const EAIPatrolState& B) {
		return (uint8)A < (uint8)B;
	});

	const int32 CurrentIndex = States.Find(PatrolStateRulesInternal.CurrentPatrolState);
	if (CurrentIndex != INDEX_NONE && CurrentIndex - 1 >= 0)
	{
		return States[CurrentIndex - 1];
	}

	return PatrolStateRulesInternal.CurrentPatrolState;
}

void UAIEnemyDetectionTask::UpdateAlpha()
{
	if (CurrentWindUpTime > 0.f)
	{
		Alpha = ElapsedTime / CurrentWindUpTime;
		Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
	}
	else
	{
		Alpha = 0.f;
	}
}

void UAIEnemyDetectionTask::TryToStopTick()
{
	if (ActiveActors.Num() > 0)
		return;
	
	if (!IsPatrolling())
		return;

	if (!IsAlphaZeroed())
		return;

	 StopTick();
}

void UAIEnemyDetectionTask::UpdateBlackBoardValue(EAIPatrolState NewValue)
{
	AIControllerInternal->GetBlackboardComponent()->SetValueAsEnum(AIControllerInternal->GetPatrolStateBlackBoardKey(), (uint8)NewValue);
}
