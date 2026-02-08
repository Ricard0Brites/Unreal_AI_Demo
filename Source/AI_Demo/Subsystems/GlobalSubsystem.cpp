// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GlobalSubsystem.h"

void UGlobalSubsystem::GetDebugModes(bool& ScreenMessages, bool& LogMessages, bool& LineTraces)
{
	ScreenMessages = (static_cast<uint8>(EDebugMode::ScreenMessages) & static_cast<uint8>(DebugState.DebugMode)) > 0;
	LogMessages = (static_cast<uint8>(EDebugMode::LogMessages) & static_cast<uint8>(DebugState.DebugMode)) > 0;
	LineTraces = (static_cast<uint8>(EDebugMode::LineTraces) & static_cast<uint8>(DebugState.DebugMode)) > 0;
}
