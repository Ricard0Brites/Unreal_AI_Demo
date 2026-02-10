// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include <type_traits> // For std::underlying_type
#include "DataTypes.generated.h"

class AAI_Demo_AIControllerBase;


#pragma region Helpers
// Function to increment/decrement an enum safely based on its underlying type
template<typename TEnum>
TEnum IncrementEnum(TEnum &EnumToIncrement, int32 Increment)
{
    // Get the underlying integer type of the enum
    using UnderlyingType = typename std::underlying_type<TEnum>::type;

    // Perform the increment/decrement on the underlying type
    UnderlyingType NewValue = static_cast<UnderlyingType>(EnumToIncrement) + Increment;

    // Cast back to the enum type
    return static_cast<TEnum>(NewValue);
}
#pragma endregion

#pragma region AI Stimuli

UENUM(BlueprintType)
enum class EAIStimulusType : uint8
{
	None UMETA(Hidden),
	Sight UMETA(DisplayName = "Sight"),
	Damage UMETA(DisplayName = "Damage"),
	Touch UMETA(DisplayName = "Touch"),
	Hearing UMETA(DisplayName = "Hearing")
};

USTRUCT(BlueprintType, Blueprintable)
struct FAIStimulusEntry : public FAIStimulus
{
	GENERATED_BODY()

	FAIStimulusEntry() = default;

	// Conversion constructor
	FAIStimulusEntry(const FAIStimulus& Other)
		: FAIStimulus(Other) // copy base
	{
	} 

	// Conversion constructor
	FAIStimulusEntry(const FAIStimulus& Other, EAIStimulusType InStimulusType)
		: FAIStimulus(Other), StimulusType(InStimulusType)
	{
	}

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AI|Perception")
	EAIStimulusType StimulusType = EAIStimulusType::None;
	
};

#pragma endregion

#pragma region AI Patrolling

UENUM(BlueprintType)
enum class EAIPatrolState : uint8
{
	None UMETA(Hidden),
	Patrolling UMETA(DisplayName = "Patrolling"),
	Suspicious UMETA(DisplayName = "Suspicious"),
	Investigating UMETA(DisplayName = "Investigating"),
	Chasing UMETA(DisplayName = "Chasing"),
	Combat UMETA(DisplayName = "Combat")
};

// AI Combat
UENUM(BlueprintType)
enum class ECombatState : uint8
{
	None UMETA(DisplayName = "None"),
    MovingToCover UMETA(DisplayName = "Moving to Cover"),
    InCover UMETA(DisplayName = "In Cover"),
    Peeking UMETA(DisplayName = "Peeking"),
    Attacking UMETA(DisplayName = "Attacking"),
    Reloading UMETA(DisplayName = "Reloading")
};

//Defines rules for state transition
UENUM(BlueprintType, meta = (Bitflags))
enum class EPatrolStateTransitionRule : uint8
{
	Block = 0 UMETA(DisplayName = "Block"),
	Next = 1 UMETA(DisplayName = "Next"),
	Previous = 1 << 1 UMETA(DisplayName = "Previous")
}; ENUM_CLASS_FLAGS(EPatrolStateTransitionRule)

// The time it takes to wind (reads as rewind) up or down the state 
USTRUCT(BlueprintType, Blueprintable)
struct FPatrolStateWindTimes
{
	GENERATED_BODY()

    FPatrolStateWindTimes() = default;

    FPatrolStateWindTimes(const EPatrolStateTransitionRule & InTransitionRules, float InWindUpTime, float InWindDownTime);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/AI_Demo.EPatrolStateTransitionRule"))
	EPatrolStateTransitionRule TransitionRules = (EPatrolStateTransitionRule::Next | EPatrolStateTransitionRule::Previous); // Can state be set to previous after a certain time?

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float WindUpTime = 5.f; // Time until we progress to the next state

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float WindDownTime = 5.f; // Time until we regress to the previous state
};

USTRUCT(BlueprintType, Blueprintable)
struct FAIEnemyDetectionTaskPayload
{
	GENERATED_BODY()

    FAIEnemyDetectionTaskPayload() = default;
    FAIEnemyDetectionTaskPayload(const EAIPatrolState& InPatrolState,
        TMap < EAIPatrolState, FPatrolStateWindTimes > InStateChangeData);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EAIPatrolState CurrentPatrolState = EAIPatrolState::Patrolling;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap < EAIPatrolState, FPatrolStateWindTimes > StateChangeData;

	EAIPatrolState UpdateCurrentPatrolState(AAI_Demo_AIControllerBase* AIController);
};

#pragma endregion