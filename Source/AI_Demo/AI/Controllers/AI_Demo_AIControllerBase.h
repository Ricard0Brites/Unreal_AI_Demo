// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "DataTypes.h"
#include "AI_Demo_AIControllerBase.generated.h"

class UAISenseConfig;
class UAIPerceptionComponent;
class UBehaviorTree;


UCLASS()
class AI_DEMO_API AAI_Demo_AIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAI_Demo_AIControllerBase();
	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;

	#pragma region AI Perception

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UAIPerceptionComponent* AIPerceptionComp = nullptr;

	// AI Perception Component Setup
	FORCEINLINE void SetupPreceptionComponent();
	FORCEINLINE void SetupPerceptionSenses();

	#pragma region AI Team
	FGenericTeamId TeamId;
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	FGenericTeamId GetGenericTeamId() const override;

	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable)
	ETeamAttitude::Type	BP_GetTeamAttitudeTowards(const AActor* Other);

	
	void AssignTeamID(APawn* InPawn);

	#pragma endregion

	#pragma region Stimulus

	UPROPERTY(BlueprintReadOnly)
	TMap<AActor*, FAIStimulusEntry> StimuliList;

	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintImplementableEvent)
	void OnActorPerceptionUpdatedEvent(AActor* Actor, FAIStimulusEntry Stimulus);

public:
	const TMap<AActor*, FAIStimulusEntry>& GetStimuliList() { return StimuliList; }		
	#pragma endregion
#pragma endregion

protected:
	
	#pragma region Behavior Tree

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior Tree")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Behavior Tree")
	FName CurrentPatrolStateBlackBoardKey = "PatrolState";
public:
	FName GetPatrolStateBlackBoardKey() {return CurrentPatrolStateBlackBoardKey;}
	#pragma endregion

};
