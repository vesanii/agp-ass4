// Fill out your copyright notice in the Description page of Project Settings.


#include "HideState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UHideState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Warning, TEXT("hide state"));
	if (Owner->CurrentPath.IsEmpty())
	{
		//get path to hiding or cover spot w.i.p
		//Owner->GetPathTo(targetlocation);
	}
}

void UHideState::Update(AEnemyCharacter* Owner, float DeltaTime)
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

	//..
	//could have action to apply healing if manual healing implemented in future

	//if player invades cover space
	if (Owner->SensedCharacter)
	{
		//if health below threshold, evade
		if (Owner->GetHealth() < 0.4f)
		{
			Owner->ChangeState(Owner->EvadeState);
			return;
		}
		else //engage player
		{
			Owner->ChangeState(Owner->EngageState);
			return;
		}
	}
}

void UHideState::Exit(AEnemyCharacter* Owner)
{
	Owner->EmptyCurrentPath();
}
