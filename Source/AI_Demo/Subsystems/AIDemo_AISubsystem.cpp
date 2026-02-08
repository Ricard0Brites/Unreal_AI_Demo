// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/AIDemo_AISubsystem.h"
#include "AI/Pathing/AIPath.h"
#include "Components/SplineComponent.h"
#include "GenericTeamAgentInterface.h"

UAIDemo_AISubsystem::UAIDemo_AISubsystem() : AITeamID((uint8)1)
{
}

AAIPath* UAIDemo_AISubsystem::GetClosestPathToLocation(FVector Location)
{
	float MinDist = (AIPaths[0]->GetSpline()->FindLocationClosestToWorldLocation(Location, ESplineCoordinateSpace::World) - Location).Length();
	AAIPath* ClosestPath = AIPaths[0];

	for (AAIPath* P : AIPaths)
	{
		float Dist = (P->GetSpline()->FindLocationClosestToWorldLocation(Location, ESplineCoordinateSpace::World) - Location).Length();


		if (Dist > MinDist)
			continue;

		MinDist = Dist;
		ClosestPath = P;
	}
	return ClosestPath;
}