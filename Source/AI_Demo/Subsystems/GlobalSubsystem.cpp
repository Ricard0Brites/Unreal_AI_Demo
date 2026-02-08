// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GlobalSubsystem.h"

void UGlobalSubsystem::GetDebugModes(bool& ScreenMessages, bool& LogMessages, bool& LineTraces)
{
	ScreenMessages = (EDebugMode::ScreenMessages & DebugState.DebugMode) > 0;
	LogMessages = (EDebugMode::LogMessages & DebugState.DebugMode) > 0;
	LineTraces = (EDebugMode::LineTraces & DebugState.DebugMode) > 0;
}
