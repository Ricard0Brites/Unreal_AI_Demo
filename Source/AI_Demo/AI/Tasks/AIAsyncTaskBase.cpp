// Fill out your copyright notice in the Description page of Project Settings.

#include "AIAsyncTaskBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

UAIAsyncTaskBase* UAIAsyncTaskBase::StartAIAsyncTask(UObject* InWorldContextObject, float InTickRatePerSecond)
{
	UAIAsyncTaskBase* AsyncTask = NewObject<UAIAsyncTaskBase>();
	AsyncTask->WorldContextObject = InWorldContextObject;
	AsyncTask->TickRatePerSecondInternal = InTickRatePerSecond;
	
	AsyncTask->RegisterWithGameInstance(InWorldContextObject);
	AsyncTask->Activate();

	return AsyncTask;
}

void UAIAsyncTaskBase::Activate()
{
	if (!WorldContextObject.IsValid())
	{
		// Cannot activate without a valid world context
		SetReadyToDestroy();
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		// Cannot activate without a valid world
		SetReadyToDestroy();
		return;
	}

	if (TickRatePerSecondInternal <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAIAsyncTaskBase: TickRatePerSecond must be greater than 0.0f. Defaulting to 10.0f."));
		TickRatePerSecondInternal = 10.0f;
	}

	float TickInterval = 1.0f / TickRatePerSecondInternal;
	World->GetTimerManager().SetTimer(TickTimerHandle, this, &UAIAsyncTaskBase::AsyncTick, TickInterval, true, 0.0f); // Loop, no initial delay
}

void UAIAsyncTaskBase::EndTask()
{
	ClearTickTimer();
	SetReadyToDestroy();
}

void UAIAsyncTaskBase::AsyncTick()
{
	// This function will be called every tick interval
	// Override this in derived classes or extend its functionality here
	// UE_LOG(LogTemp, Log, TEXT("UAIAsyncTaskBase::AsyncTick() called."));
}

void UAIAsyncTaskBase::ClearTickTimer()
{
	if (WorldContextObject.IsValid())
	{
		UWorld* World = WorldContextObject->GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(TickTimerHandle);
		}
	}
}
