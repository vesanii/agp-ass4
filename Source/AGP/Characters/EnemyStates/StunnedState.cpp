// Fill out your copyright notice in the Description page of Project Settings.


#include "StunnedState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UStunnedState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Stunned"));
	if (!Owner->CurrentPath.IsEmpty())
	{
		Owner->EmptyCurrentPath();
	}
	StunTimer = 3.0f;
}

void UStunnedState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
	//die
	if (Owner->HasDied())
	{
		Owner->ChangeState(Owner->DeadState);
		return;
	}
	StunTimer -= DeltaTime;
	//character is stunned for 3 seconds and cannot change state until timer is finished
	if (StunTimer <= 0)
	{
		if (!Owner->SensedCharacter)
		{
			Owner->ChangeState(Owner->PatrolState);
			return;
		}
		else
		{
			if (Owner->GetHealth() >= 0.4f)
			{
				Owner->ChangeState(Owner->EngageState);
				return;
			}
			else
			{
				Owner->ChangeState(Owner->EvadeState);
				return;
			}
		}
	}
}

void UStunnedState::Exit(AEnemyCharacter* Owner)
{
	Owner->ResetStun();
}
