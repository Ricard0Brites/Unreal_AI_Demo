// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Controllers/AI_Demo_AIControllerBase.h"

FPatrolStateWindTimes::FPatrolStateWindTimes(int32 InTransitionRules, float InWindUpTime, float InWindDownTime) :
    TransitionRules(InTransitionRules),
    WindUpTime(InWindUpTime), 
    WindDownTime(InWindDownTime)
{

}

FAIEnemyDetectionTaskPayload::FAIEnemyDetectionTaskPayload(const EAIPatrolState& InPatrolState, TMap < EAIPatrolState, FPatrolStateWindTimes > InStateChangeData)
    :
    CurrentPatrolState(InPatrolState), StateChangeData(InStateChangeData)
{

}

EAIPatrolState FAIEnemyDetectionTaskPayload::UpdateCurrentPatrolState(AAI_Demo_AIControllerBase* AIController)
{
    if (!AIController)
        return EAIPatrolState::None;

   CurrentPatrolState = static_cast<EAIPatrolState>(AIController->GetBlackboardComponent()->GetValueAsEnum(AIController->GetPatrolStateBlackBoardKey()));

   return CurrentPatrolState;
}
