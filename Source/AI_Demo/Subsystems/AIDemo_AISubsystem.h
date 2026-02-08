// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AIDemo_AISubsystem.generated.h"

class AAIPath;
struct FGenericTeamId;

UCLASS()
class AI_DEMO_API UAIDemo_AISubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<AAIPath*> AIPaths;

	UFUNCTION(BlueprintCallable)
	AAIPath* GetClosestPathToLocation(FVector Location);


	const FGenericTeamId& GetAITeamID();

private:
	FGenericTeamId AITeamId; // Single Team - All AI is pooled as friendly towards each other
	
};
