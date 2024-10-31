// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UDeadState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Dead"));
	if (!Owner->CurrentPath.IsEmpty())
	{
		Owner->EmptyCurrentPath();
	}
	Owner->OnDeath();
}

void UDeadState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
	TimeUntilDestroy -= DeltaTime;
	if (TimeUntilDestroy <= 0)
	{
		Owner->Destroy();
	}
}

