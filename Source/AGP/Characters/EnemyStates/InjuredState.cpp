#include "InjuredState.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"
#include "AGP/Pathfinding/NavigationNode.h"

void UInjuredState::Entry(AEnemyCharacter* Owner)
{
    UE_LOG(LogTemp, Display, TEXT("State: Injured"));
    
    if (Owner->PathfindingSubsystem)
    {
        // find the closest cover node and health pickup
        ANavigationNode* ClosestCoverNode = Owner->PathfindingSubsystem->FindNearestCoverNode(Owner->GetActorLocation());
        Owner->FindHealthPickup();
        AHealthPickup* ClosestHealthPickup = Owner->SensedHealUp.Get();

        if (ClosestHealthPickup && ClosestCoverNode)
        {
            float DistanceToCover = FVector::Distance(Owner->GetActorLocation(), ClosestCoverNode->GetActorLocation());
            float DistanceToHealth = FVector::Distance(Owner->GetActorLocation(), ClosestHealthPickup->GetActorLocation());

            if (DistanceToCover < DistanceToHealth)
            {
                Owner->CreatePathTo(ClosestCoverNode->GetActorLocation());
            }
            else
            {
                Owner->CreatePathTo(ClosestHealthPickup->GetActorLocation());
            }
        }
        else if (ClosestCoverNode)
        {
            Owner->CreatePathTo(ClosestCoverNode->GetActorLocation());
        }
        else if (ClosestHealthPickup)
        {
            Owner->CreatePathTo(ClosestHealthPickup->GetActorLocation());
        }
    }
}

void UInjuredState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
    // death or stun
    if (Owner->HasDied())
    {
        Owner->ChangeState(Owner->DeadState);
        return;
    }
    if (Owner->IsStunned())
    {
        Owner->ChangeState(Owner->StunnedState);
        return;
    }

    // check if at cover node or health pickup
    if (Owner->CurrentPath.IsEmpty())
    {
        if (Owner->GetHealth() < 0.8f && Owner->PathfindingSubsystem->FindNearestCoverNode(Owner->GetActorLocation()))
        {
            Owner->ChangeState(Owner->HideState);
            return;
        }
        else if (Owner->GetHealth() >= 0.8f)
        {
            Owner->ChangeState(Owner->PatrolState);
            return;
        }
    }

    Owner->MoveAlongPath();
}

void UInjuredState::Exit(AEnemyCharacter* Owner)
{
    Owner->EmptyCurrentPath();
}
