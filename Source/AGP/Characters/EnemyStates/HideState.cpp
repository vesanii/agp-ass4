#include "HideState.h"
#include "AGP/Characters/EnemyCharacter.h"
#include "AGP/Characters/HealthComponent.h"

void UHideState::Entry(AEnemyCharacter* Owner)
{
    UE_LOG(LogTemp, Warning, TEXT("State: Hide"));
    if (Owner->CurrentPath.IsEmpty())
    {
        // already in cover
    }
    
    PreviousHealth = Owner->HealthComponent->GetCurrentHealth();
}

void UHideState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
    // death or stun first
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

    // check for damage 
    float CurrentHealth = Owner->HealthComponent->GetCurrentHealth();
    if (CurrentHealth < PreviousHealth)
    {
        Owner->ChangeState(Owner->EvadeState);
        return;
    }
    PreviousHealth = CurrentHealth;  

    // regen 
    Owner->HealthComponent->ApplyHealing(DeltaTime * PassiveRegenRate);

    // transition through heal
    if (Owner->GetHealth() >= 0.75f)
    {
        Owner->ChangeState(Owner->PatrolState);
        return;
    }
}

void UHideState::Exit(AEnemyCharacter* Owner)
{
    Owner->EmptyCurrentPath();
}
