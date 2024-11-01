// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadState.h"
#include "AGP/Characters/EnemyCharacter.h"
#include <AGP/MultiplayerGameMode.h>

void UDeadState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Display, TEXT("State: Dead"));
	if (!Owner->CurrentPath.IsEmpty())
	{
		Owner->EmptyCurrentPath();
	}
	Owner->OnEnemyDeath();
}

void UDeadState::Update(AEnemyCharacter* Owner, float DeltaTime)
{
	TimeUntilDestroy -= DeltaTime;
	if (TimeUntilDestroy <= 0)
	{
		if (AMultiplayerGameMode* GameMode = Cast<AMultiplayerGameMode>(GetWorld()->GetAuthGameMode()))
		{
			// Tell the GameMode to respawn this enemy.
			GameMode->RespawnEnemy(Owner->GetController());
		}
	}
}

