// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadState.h"
#include "AGP/Characters/EnemyCharacter.h"

void UDeadState::Entry(AEnemyCharacter* Owner)
{
	UE_LOG(LogTemp, Warning, TEXT("State: Dead"));
	if (!Owner->CurrentPath.IsEmpty())
	{
		Owner->EmptyCurrentPath();
	}
	Owner->Destroy();
}

