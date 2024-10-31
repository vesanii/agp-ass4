// Fill out your copyright notice in the Description page of Project Settings.

#include "EvadeState.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UEvadeState::Entry(AEnemyCharacter* Owner)
{
    UE_LOG(LogTemp, Display, TEXT("State: Evade"));
    bCoverNodeAssigned = false;
    
    // find cover node
    if (Owner->GetHealth() < 0.4f && Owner->PathfindingSubsystem)
    {
        ANavigationNode* NearestCoverNode = Owner->PathfindingSubsystem->FindNearestCoverNode(Owner->GetActorLocation());
        if (NearestCoverNode)
        {
            Owner->CreatePathTo(NearestCoverNode->GetActorLocation());
            bCoverNodeAssigned = true;
            UE_LOG(LogTemp, Display, TEXT("Navigating to cover node at location: %s"), *NearestCoverNode->GetActorLocation().ToString());
        }
    }
    else
    {
        Owner->CreatePathAwayFrom(Owner->SensedCharacter.Get());
    }
}

void UEvadeState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
    if (!Owner) return;

    // died
    if (Owner->HasDied())
    {
        Owner->ChangeState(Owner->DeadState);
        return;
    }

    // when at cover node, hide
    if (bCoverNodeAssigned && Owner->CurrentPath.IsEmpty())
    {
        Owner->ChangeState(Owner->HideState);
        return;
    }

    // else
    if (!bCoverNodeAssigned)
    {
        if (Owner->SensedCharacter.IsValid() && Owner->GetHealth() >= 0.4f)
        {
            Owner->ChangeState(Owner->EngageState);
            return;
        }

        if (Owner->CurrentPath.IsEmpty() && Owner->SensedCharacter.IsValid())
        {
            Owner->CreatePathAwayFrom(Owner->SensedCharacter.Get());
        }
    }

    Owner->MoveAlongPath();
}

void UEvadeState::Exit(AEnemyCharacter* Owner)
{
    Owner->EmptyCurrentPath();
}
