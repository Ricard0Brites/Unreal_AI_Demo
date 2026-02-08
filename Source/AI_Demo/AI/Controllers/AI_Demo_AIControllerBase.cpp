// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controllers/AI_Demo_AIControllerBase.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionComponent.h"
#include "Subsystems/AIDemo_AISubsystem.h"
#include "GameFramework/Pawn.h"

AAI_Demo_AIControllerBase::AAI_Demo_AIControllerBase() : TeamId((uint8)0)
{
	SetupPreceptionComponent();
	SetupPerceptionSenses();
}

void AAI_Demo_AIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAI_Demo_AIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAI_Demo_AIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AssignTeamID(InPawn);
}

void AAI_Demo_AIControllerBase::SetupPreceptionComponent()
{
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &AAI_Demo_AIControllerBase::OnPerceptionUpdated);
}

void AAI_Demo_AIControllerBase::SetupPerceptionSenses()
{
	check(AIPerceptionComp);

	// Sight
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1000.0f; 
		SightConfig->LoseSightRadius = 1500.0f;
		SightConfig->PeripheralVisionAngleDegrees = 70.0f; 
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		AIPerceptionComp->ConfigureSense(*SightConfig);
	}

	// Hearing
	UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 1100.0f;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		AIPerceptionComp->ConfigureSense(*HearingConfig);
	}

	// Damage
	UAISenseConfig_Damage* DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	if (DamageConfig)
		AIPerceptionComp->ConfigureSense(*DamageConfig);

	// Dominant Sense
	AIPerceptionComp->SetDominantSense(UAISenseConfig_Sight::StaticClass());
}

void AAI_Demo_AIControllerBase::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
}

void AAI_Demo_AIControllerBase::AssignTeamID(APawn* InPawn)
{
	if (IGenericTeamAgentInterface* TeamInterface = Cast< IGenericTeamAgentInterface>(InPawn))
	{
		UAIDemo_AISubsystem* AISubSys = GetWorld()->GetSubsystemChecked<UAIDemo_AISubsystem>();
		TeamInterface->SetGenericTeamId(AISubSys->AITeamID);
	}
}

void AAI_Demo_AIControllerBase::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamId = InTeamID;
}

FGenericTeamId AAI_Demo_AIControllerBase::GetGenericTeamId() const
{
	return TeamId;
}

ETeamAttitude::Type AAI_Demo_AIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{

	if(const APawn* CharacterRef = Cast<APawn>(&Other))
		if (const AAI_Demo_AIControllerBase* C = Cast<AAI_Demo_AIControllerBase>(CharacterRef->GetController()))
			return  C->GetGenericTeamId() == TeamId ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;

	return ETeamAttitude::Hostile;
}

ETeamAttitude::Type AAI_Demo_AIControllerBase::BP_GetTeamAttitudeTowards(const AActor* Other)
{
	return GetTeamAttitudeTowards(*Other);
}
