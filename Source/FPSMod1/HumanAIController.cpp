// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanAIController.h"
#include "NavigationSystem.h"

void AHumanAIController::BeginPlay()
{
    Super::BeginPlay();

    NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

    RandomPatrol();
}

void AHumanAIController::RandomPatrol()
{
    if (NavArea)
    {
        NavArea->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(),
            RandomLocation, 10000.0f);

        MoveToLocation(RandomLocation);
    }
}
