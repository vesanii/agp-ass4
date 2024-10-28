// Fill out your copyright notice in the Description page of Project Settings.


#include "InjuredState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UInjuredState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Injured"));
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedHealUp)
	{
		Owner->CreatePathTo(Owner->SensedHealUp);
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
	if (Owner->SensedCharacter && Owner->GetHealth() < 0.4f)
	{
		Owner->ChangeState(Owner->EvadeState);
		return;
	}
	//if health pickup acquired, lost, or path expended, return to patrol
	if (Owner->GetHealth() >= 0.4f || !Owner->SensedHealUp || Owner->CurrentPath.IsEmpty()) 
	{
		Owner->ChangeState(Owner->PatrolState);
		return;
	}
	Owner->MoveAlongPath();
}

void UInjuredState::Exit(AEnemyCharacter* Owner)
{
	Owner->SensedHealUp = nullptr;
	Owner->EmptyCurrentPath();
}
