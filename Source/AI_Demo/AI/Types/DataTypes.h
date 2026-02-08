// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AIPerceptionTypes.h"
#include "DataTypes.generated.h"


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