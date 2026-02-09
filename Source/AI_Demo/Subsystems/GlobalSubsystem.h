// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/Tuple.h"
#include "GlobalSubsystem.generated.h"

#pragma region Debug
UENUM(Blueprintable, BlueprintType, meta = (Bitflags))
enum class EDebugSystems : uint8
{
	Disabled = 0,
	Interaction = 1,
	Weapon = 1 << 1,
}; ENUM_CLASS_FLAGS(EDebugSystems)

UENUM(Blueprintable, BlueprintType, meta = (Bitflags))
enum class EDebugMode : uint8
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "/Script/AIDemo.EDebugSystems"))
	EDebugSystems SystemsToDebug;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "/Script/AIDemo.EDebugMode"))
	EDebugMode DebugMode;
};

#pragma endregion

UCLASS()
class AI_DEMO_API UGlobalSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	bool IsDebugEnabled(EDebugSystems System) { return (static_cast<uint8>(DebugState.SystemsToDebug) & static_cast<uint8>(System)) != 0; }

	UFUNCTION(BlueprintPure)
	void GetDebugModes(bool &ScreenMessages, bool &LogMessages, bool &LineTraces);

	UPROPERTY(BlueprintReadWrite)
	FDebugPair DebugState;
};
