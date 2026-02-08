// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "AI_Demo_AIControllerBase.generated.h"

class UAISenseConfig;
class UAIPerceptionComponent;

UCLASS()
class AI_DEMO_API AAI_Demo_AIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAI_Demo_AIControllerBase();
	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;

	#pragma region AI Perception

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UAIPerceptionComponent* AIPerceptionComp = nullptr;

#pragma endregion


	void OnPossess(APawn* InPawn) override;

private:
	// AI Perception Component Setup
	FORCEINLINE void SetupPreceptionComponent();
	FORCEINLINE void SetupPerceptionSenses();

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*> &UpdatedActors);
	void AssignTeamID(APawn* InPawn);

	#pragma region AI Team

	FGenericTeamId TeamId;
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	FGenericTeamId GetGenericTeamId() const override;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	UFUNCTION(BlueprintCallable)
	ETeamAttitude::Type	BP_GetTeamAttitudeTowards(const AActor* Other);
	#pragma endregion


};
