// Fill out your copyright notice in the Description page of Project Settings.


#include "InjuredState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UInjuredState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Injured"));
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedHealUp.IsValid())
	{
		Owner->CreatePathTo(Owner->SensedHealUp.Get());
	}
}

void UInjuredState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
	//die
	if (Owner->HasDied())
	{
		Owner->ChangeState(Owner->DeadState);
		return;
	}
	//regardless of finding heal pickup, evade if player encountered while inured
	if (Owner->SensedCharacter.IsValid() && Owner->GetHealth() < 0.4f)
	{
		Owner->ChangeState(Owner->EvadeState);
		return;
	}
	//if health pickup acquired, lost, or path expended, return to patrol
	if (Owner->GetHealth() >= 0.4f || !Owner->SensedHealUp.IsValid()) 
	{
		Owner->ChangeState(Owner->PatrolState);
		return;
	}
	//renew path if it runs out before state change
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedHealUp.IsValid())
	{
		Owner->CreatePathTo(Owner->SensedHealUp.Get());
	}
	Owner->MoveAlongPath();
}

void UInjuredState::Exit(AEnemyCharacter* Owner)
{
	Owner->EmptyCurrentPath();
}
