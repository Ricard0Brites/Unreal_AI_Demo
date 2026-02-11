// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Perception/AIPerceptionTypes.h"
#include "DataTypes.h"
#include "AIEnemyDetectionTask.generated.h"

class UAIPerceptionComponent;
class AAI_Demo_AIControllerBase;


UCLASS()
class AI_DEMO_API UAIEnemyDetectionTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	#pragma region Async Task
protected:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "AI Async Tasks", WorldContext = "InWorldContextObject"))
	static UAIEnemyDetectionTask* StartAIEnemyDetectionTask(
		UObject* InWorldContextObject = nullptr,
		float InTickRatePerSecond = 10.0f,
		UAIPerceptionComponent* InPerceptionComponent = nullptr,
		const FAIEnemyDetectionTaskPayload &InPatrolStateRules = FAIEnemyDetectionTaskPayload());

	void Activate() override;
#pragma endregion
	
	void EndTask();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskStarted, UAIEnemyDetectionTask*, AsyncTask);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPatrolStateUpdate, EAIPatrolState, NewState);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlphaUpdate, float, Alpha);

	UPROPERTY(BlueprintAssignable)
	FOnTaskStarted OnTaskStarted;

	UPROPERTY(BlueprintAssignable)
	FOnPatrolStateUpdate OnPatrolStateUpdate;

	UPROPERTY(BlueprintAssignable)
	FOnAlphaUpdate OnAlphaUpdate;

	#pragma region Inputs
protected:
	UPROPERTY()
	TWeakObjectPtr<UObject> WorldContextObject = nullptr;
	UPROPERTY()
	float TickRatePerSecondInternal = 10.f;
	UPROPERTY()
	UAIPerceptionComponent* PerceptionComponentInternal = nullptr;
	UPROPERTY()
	FAIEnemyDetectionTaskPayload PatrolStateRulesInternal = FAIEnemyDetectionTaskPayload();
#pragma endregion

	#pragma region Perception Component Events
	UFUNCTION()
	void OnTargetUpdated(AActor* InActor, FAIStimulus InStimulus);

	#pragma endregion

	#pragma region Internal Values
	UWorld* WorldRefInternal = nullptr;
	AAI_Demo_AIControllerBase* AIControllerInternal = nullptr;
	float CurrentWindUpTime = -1, CurrentWindDownTime = -1;
#pragma endregion

	#pragma region Async Tick 
private:
	
	FTimerHandle TickTimerHandle;
	void StopTick();
	void StartTick();

	bool bIsTickEnabled = false;

protected:
	UFUNCTION()
	virtual void AsyncTick();
#pragma endregion

	// ----------- Business Logic

protected:
	UPROPERTY(BlueprintReadOnly)
	TSet<AActor*> ActiveActors = {};

private:
	float ElapsedTime = 0;
	float Alpha = 0;
	bool HasStartedWindDown = false;

	bool TryGetWorld(); // Validation
	bool TryGetPerceptionComponent(); // Validation
	void BindPerceptionEvents(); // Event Binding
	void ValidateTickRate(); // Validation
	void TryToStopTick();
	bool TryGetAIController(); // Getter

	// Helpers
	bool IsPatrolling();
	bool IsEnemyDetected();
	bool IsAlphaZeroed();
	
	//States
	void MoveToNextState();
	void MoveToPreviousState();
	bool CanRegressOneState();
	bool CanForwardOneState();
	EAIPatrolState GetNextState();
	EAIPatrolState GetPreviousState();

	//Timings
	float GetCurrentWindUpTime();
	float GetCurrentWindDownTime();

	// Timers
	void StartWindDown();
	void AbortWindDown();
	void OnWindDownFinished();
	FTimerHandle WindDownTimerHandle;
	
	//Values
	void UpdateAlpha();
	void UpdateBlackBoardValue(EAIPatrolState NewValue);
};