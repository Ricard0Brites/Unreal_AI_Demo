// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/Tuple.h"
#include "GlobalSubsystem.generated.h"

#pragma region Debug
UENUM(Blueprintable, BlueprintType, meta = (Bitflags))
enum EDebugSystems : uint8
{
	None = 0,
	Interaction = 1,
}; ENUM_CLASS_FLAGS(EDebugSystems)

UENUM(Blueprintable, BlueprintType, meta = (Bitflags))
enum EDebugMode : uint8
{
	Disabled,
	ScreenMessages,
	LogMessages,
	LineTraces,
}; ENUM_CLASS_FLAGS(EDebugMode)

USTRUCT(BlueprintType, Blueprintable)
struct FDebugPair
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (Bitmask))
	TEnumAsByte<EDebugSystems> SystemsToDebug = TEnumAsByte<EDebugSystems>(EDebugSystems::None);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (Bitmask))
	TEnumAsByte<EDebugMode> DebugMode = TEnumAsByte<EDebugMode>(EDebugMode::Disabled);
};

#pragma endregion

UCLASS()
class AI_DEMO_API UGlobalSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	bool IsDebugEnabled(EDebugSystems System) { return (System & DebugState.SystemsToDebug) > 0; }

	UFUNCTION(BlueprintPure)
	void GetDebugModes(bool &ScreenMessages, bool &LogMessages, bool &LineTraces);

	UPROPERTY(BlueprintReadWrite)
	FDebugPair DebugState;
};
