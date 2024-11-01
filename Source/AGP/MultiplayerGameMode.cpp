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
			for (TActorIterator<APlayerStart> StartSpot(GetWorld()); StartSpot; ++StartSpot)
			{
				if (StartSpot->PlayerStartTag == FName("PlayerStart"))
				{
					RestartPlayerAtPlayerStart(Controller, *StartSpot);
				}
			}
			if (APlayerCharacter* NewPossessedCharacter = Cast<APlayerCharacter>(Controller->GetPawn()))
			{
				NewPossessedCharacter->ChooseCharacterMesh();
				NewPossessedCharacter->DrawUI();
			}
		}
	}
}
