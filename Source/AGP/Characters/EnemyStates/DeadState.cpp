// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadState.h"
#include "AGP/Characters/EnemyCharacter.h"
#include <AGP/AGPGameInstance.h>

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
		if (UAGPGameInstance* AGPGameInstance = Cast<UAGPGameInstance>(GetWorld()->GetGameInstance()))
		{
			// Tell the GameMode to respawn this enemy.
			AGPGameInstance->RespawnEnemy(Owner->GetController());
		}
	}
}

