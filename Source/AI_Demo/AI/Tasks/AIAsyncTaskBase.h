// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AIAsyncTaskBase.generated.h"

// Forward Declaration
// class UWorld; // Not needed, already included in .cpp

UCLASS()
class AI_DEMO_API UAIAsyncTaskBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	// Static factory method to create and activate the async task
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", Category="AI Async Tasks", WorldContext="InWorldContextObject"))
	static UAIAsyncTaskBase* StartAIAsyncTask(UObject* InWorldContextObject, float InTickRatePerSecond = 10.0f);

	// UBlueprintAsyncActionBase override
	virtual void Activate() override;

	// Helper to explicitly end the task and clean up the timer
	UFUNCTION(BlueprintCallable, Category="AI Async Tasks")
	void EndTask();

protected:
	// Internal tick function, called by the timer
	virtual void AsyncTick();

private:
	UPROPERTY()
	TWeakObjectPtr<UObject> WorldContextObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI Async Tasks", meta=(AllowPrivateAccess="true"))
	float TickRatePerSecondInternal;

	FTimerHandle TickTimerHandle;

	// Helper to clear the timer
	void ClearTickTimer();
};
