// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIPath.generated.h"

class USplineComponent;

UCLASS()
class AI_DEMO_API AAIPath : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIPath();

	UFUNCTION(BlueprintPure)
	AAIPath* GetNextPath(AActor* ActorOnPath);

	UFUNCTION(BlueprintPure)
	FVector GetNextLocationInRange(const APawn *ControlledPawn, float Range, bool InvertDirection);

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	AAIPath* NextPath = nullptr;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	AAIPath* PreviousPath = nullptr;

	USplineComponent* GetSpline() { return Spline; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineComponent* Spline = nullptr;


};
