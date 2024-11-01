// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "Characters/PlayerCharacter.h"
#include "Characters/EnemyCharacter.h"
#include <Engine.h>

void AMultiplayerGameMode::RespawnPlayer(AController* Controller)
{
	if (Controller)
	{
		if (APlayerCharacter* CurrentlyPossessedCharacter = Cast<APlayerCharacter>(Controller->GetPawn()))
		{
			Controller->UnPossess();
			CurrentlyPossessedCharacter->Destroy();
			RestartPlayer(Controller);
			if (APlayerCharacter* NewPossessedCharacter = Cast<APlayerCharacter>(Controller->GetPawn()))
			{
				NewPossessedCharacter->ChooseCharacterMesh();
				NewPossessedCharacter->DrawUI();
			}
		}
	}
}

void AMultiplayerGameMode::RespawnEnemy(AController* Controller)
{
	if (Controller)
	{
		if (AEnemyCharacter* CurrentEnemyCharacter = Cast<AEnemyCharacter>(Controller->GetPawn()))
		{
			Controller->Destroy();
			CurrentEnemyCharacter->Destroy();
			for (TActorIterator<APlayerStart> StartSpot(GetWorld()); StartSpot; ++StartSpot)
			{
				if (StartSpot->PlayerStartTag == FName("Enemy"))
				{
					AEnemyCharacter* NewEnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyCharacterClass.Get(), StartSpot->GetActorLocation(), FRotator::ZeroRotator);
				}
			}
		}
	}
}
