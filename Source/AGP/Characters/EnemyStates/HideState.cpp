// Fill out your copyright notice in the Description page of Project Settings.

#include "HideState.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Characters/HealthComponent.h"

void UHideState::Entry(AEnemyCharacter* Owner)
{
    UE_LOG(LogTemp, Warning, TEXT("State: Hide"));
    HealingRate = 5.0f; // healing rate
}

void UHideState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
    if (!Owner || !Owner->HealthComponent) return;

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

// healing and heal exit
    if (Owner->HealthComponent->GetCurrentHealthPercentage() < 0.75f)
    {
        Owner->HealthComponent->ApplyHealing(HealingRate * DeltaTime); 
    }
    else
    {
        Owner->ChangeState(Owner->PatrolState);
    }
}

void UHideState::Exit(AEnemyCharacter* Owner)
{
    if (Owner)
    {
        Owner->EmptyCurrentPath();
    }
}
