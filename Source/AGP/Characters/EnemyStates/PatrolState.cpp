// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UPatrolState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Patrol"));
	if(Owner)
	{
		if (Owner->CurrentPath.IsEmpty())
		{
			Owner->CreateRandomPath();
		}
	}
}

void UPatrolState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
	//die
	if (Owner->HasDied())
	{
		Owner->ChangeState(Owner->DeadState);
		return;
	}

	//the following pickup checks are temporary until proper ai senses for weapons and health pickups are implemented
	//search for weapon if required
	if (!Owner->HasWeapon() && !Owner->SensedWeapon)
	{
		Owner->FindWeaponPickup();
	}
	//search for heal up if required
	if (Owner->GetHealth() < 0.4f && !Owner->SensedHealUp)
	{
		Owner->FindHealthPickup();
	}

	//if character receives more than 15 damage at once at full health, they are stunned
	if (Owner->IsStunned()) 
	{
		Owner->ChangeState(Owner->StunnedState);
		return;
	}
	//if weapon found and none is equipped, go towards the weapon
	if (!Owner->HasWeapon() && Owner->SensedWeapon)
	{
		Owner->ChangeState(Owner->UnarmedState);
		return;
	}
	//if available, look for health regen
	if (Owner->GetHealth() < 0.4f && !Owner->SensedCharacter.IsValid() && Owner->SensedHealUp)
	{
		Owner->ChangeState(Owner->InjuredState);
		return;
	}
	//engage with found player given enough health
	if (Owner->SensedCharacter.IsValid() && Owner->GetHealth() >= 0.4f && Owner->HasWeapon())
	{
		Owner->ChangeState(Owner->EngageState);
		return;
	}
	//evade player when health falls below 0.4
	if (Owner->SensedCharacter.IsValid() && Owner->GetHealth() < 0.4f)
	{
		Owner->ChangeState(Owner->EvadeState);
		return;
	}
	//renew path if it runs out before state change
	if (Owner)
	{
		if (Owner->CurrentPath.IsEmpty())
		{
			Owner->CreateRandomPath();
		}
	}
	Owner->MoveAlongPath();
}

void UPatrolState::Exit(AEnemyCharacter* Owner)
{
	Owner->EmptyCurrentPath();
}
