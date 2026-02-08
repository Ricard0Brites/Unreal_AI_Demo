// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Pathing/AIPath.h"
#include "Components/SplineComponent.h"
#include "Subsystems/AIDemo_AISubsystem.h"

AAIPath::AAIPath()
{
	PrimaryActorTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	Spline->SetupAttachment(RootComponent, TEXT(""));

}

AAIPath* AAIPath::GetNextPath(AActor* ActorOnPath)
{
	check(ActorOnPath);

	float Dist = Spline->GetDistanceAlongSplineAtLocation(ActorOnPath->GetActorLocation(), ESplineCoordinateSpace::World);
	FVector Dir = Spline->GetDirectionAtDistanceAlongSpline(Dist, ESplineCoordinateSpace::World);

	return Dir.Dot(ActorOnPath->GetActorForwardVector()) > 0 ? NextPath : PreviousPath;
}

FVector AAIPath::GetNextLocationInRange(const APawn *ControlledPawn, float Range, bool InvertDirection)
{
	// Location in Spline
	FVector Origin = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);

	// Spline Direction
	FVector Direction = Spline->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
	if (InvertDirection)
	{
		Direction.X *= -1;
		Direction.Y *= -1;
	}

	// Next Location
	return Spline->FindLocationClosestToWorldLocation(Origin + (Direction * Range), ESplineCoordinateSpace::World);
}

void AAIPath::BeginPlay()
{
	Super::BeginPlay();
	
	if (UWorld* WorldRef = GetWorld())
		if (UAIDemo_AISubsystem* AISubsystem = WorldRef->GetSubsystemChecked<UAIDemo_AISubsystem>())
			AISubsystem->AIPaths.AddUnique(this);

}
