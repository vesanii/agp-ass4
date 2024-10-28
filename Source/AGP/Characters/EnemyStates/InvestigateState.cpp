// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigateState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UInvestigateState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Investigate"));
	if (Owner->CurrentPath.IsEmpty())
	{
		Owner->CreatePathTo(Owner->LastKnownCharacterLocation);
	}
	InvestigateTimer = 5.0f;
}

void UInvestigateState::Update(AEnemyCharacter* Owner, float DeltaTime)
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
	//if investigation successful 
	if (Owner->SensedCharacter)
	{
		//if enough health, engage
		if (Owner->GetHealth() >= 0.4)
		{
			Owner->ChangeState(Owner->EngageState);
			return;
		}
		else //if health falls below 0.4, evade
		{
			Owner->ChangeState(Owner->EvadeState);
			return;
		}
	}
	//if investigation fails, return to patrol
	if (!Owner->SensedCharacter && InvestigateTimer <= 0)
	{
		Owner->ChangeState(Owner->PatrolState);
		return;
	}

	//once at last known location, investigate for 5 seconds
	if (Owner->CurrentPath.IsEmpty() && InvestigateTimer > 0)
	{
		InvestigateTimer -= DeltaTime;
		Owner->Investigate(DeltaTime);
	}
	Owner->MoveAlongPath();
}

void UInvestigateState::Exit(AEnemyCharacter* Owner)
{
	Owner->EmptyCurrentPath();
}
