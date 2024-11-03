// Fill out your copyright notice in the Description page of Project Settings.


#include "EvadeState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UEvadeState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Evade"));
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedCharacter.IsValid())
	{
		Owner->CreatePathAwayFrom(Owner->SensedCharacter.Get());
	}
	DistanceFromPlayer = 0.0f;
}

void UEvadeState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
	//die
	if (Owner->HasDied())
	{
		Owner->ChangeState(Owner->DeadState);
		return;
	}
	//update distance from player
	DistanceFromPlayer = FVector::Distance(Owner->GetActorLocation(), Owner->LastKnownCharacterLocation);
	//if the player is lost and health is still below threshold
	if (!Owner->SensedCharacter.IsValid() && Owner->GetHealth() < 0.4)
	{
			
		//return to patrol if no health pick up is found
		if (!Owner->SensedHealUp.IsValid())
		{
			if (DistanceFromPlayer >= 1000.0f)
			{
				Owner->ChangeState(Owner->InjuredState);
				return;
			}
		}
	}
	//if health somehow regained, return to engage
	if (Owner->SensedCharacter.IsValid() && Owner->GetHealth() >= 0.4f)
	{
		Owner->ChangeState(Owner->EngageState);
		return;
	}
	
	//renew path if it runs out before state change
	if (Owner->CurrentPath.IsEmpty() && Owner->SensedCharacter.IsValid())
	{
		Owner->CreatePathAwayFrom(Owner->SensedCharacter.Get());
	}
	Owner->MoveAlongPath();
}

void UEvadeState::Exit(AEnemyCharacter* Owner)
{
	Owner->EmptyCurrentPath();
}
