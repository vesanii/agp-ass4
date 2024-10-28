// Fill out your copyright notice in the Description page of Project Settings.


#include "UnarmedState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UUnarmedState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Unarmed"));
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedWeapon)
	{
		Owner->CreatePathTo(Owner->SensedWeapon);
	}
}

void UUnarmedState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
	//die
	if (Owner->HasDied())
	{
		Owner->ChangeState(Owner->DeadState);
		return;
	}
	//if character receives more than 15 damage at one time at full health, they are stunned
	if (Owner->IsStunned())
	{
		Owner->ChangeState(Owner->StunnedState);
		return;
	}
	//regardless of weapon, evade if health falls below 0.4
	if (Owner->SensedCharacter && Owner->GetHealth() < 0.4)
	{
		Owner->ChangeState(Owner->EvadeState);
		return;
	}
	//return to patrol if any of the conditions are met
	if (Owner->HasWeapon() || !Owner->SensedWeapon || Owner->CurrentPath.IsEmpty()) {
		Owner->ChangeState(Owner->PatrolState);
		return;
	}
	//renew path if it runs out before state change
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedWeapon)
	{
		Owner->CreatePathTo(Owner->SensedWeapon);
	}
	Owner->MoveAlongPath();
}

void UUnarmedState::Exit(AEnemyCharacter* Owner)
{
	Owner->SensedWeapon = nullptr;
	Owner->EmptyCurrentPath();
}
