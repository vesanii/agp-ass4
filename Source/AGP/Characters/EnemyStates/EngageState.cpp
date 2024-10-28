// Fill out your copyright notice in the Description page of Project Settings.


#include "EngageState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UEngageState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Engage"));
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedCharacter)
	{
		Owner->CreatePathTo(Owner->SensedCharacter);
	}
}

void UEngageState::Update(AEnemyCharacter* Owner, float DeltaTime)
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
	//if the player is lost, go to investigate
	if (!Owner->SensedCharacter)
	{
		Owner->ChangeState(Owner->InvestigateState);
		return;
	}
	//if health falls below 0.4, evade
	if (Owner->GetHealth() < 0.4f)
	{
		Owner->ChangeState(Owner->EvadeState);
		return;
	}

	if (Owner->SensedCharacter)
	{
		Owner->Fire(Owner->SensedCharacter->GetActorLocation());
	}
	if (Owner->WeaponComponent)
	{
		if (Owner->WeaponComponent->IsMagazineEmpty()) {
			Owner->WeaponComponent->ReloadMagazine();
		}
	}
	//renew path if it runs out before state change
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedCharacter)
	{
		Owner->CreatePathTo(Owner->SensedCharacter);
	}
	Owner->MoveAlongPath();
}

void UEngageState::Exit(AEnemyCharacter* Owner)
{
	Owner->EmptyCurrentPath();
}
