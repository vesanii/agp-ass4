// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPGameInstance.h"
#include "Characters/EnemyCharacter.h"

UClass* UAGPGameInstance::GetWeaponPickupClass() const
{
	return WeaponPickupClass.Get();
}

UClass* UAGPGameInstance::GetHealthPickupClass() const
{
	return HealthPickupClass.Get();
}

void UAGPGameInstance::SpawnGroundHitParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundHitParticles, SpawnLocation);
}

void UAGPGameInstance::SpawnBloodSpatterParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSpatterEffect, SpawnLocation);
}

void UAGPGameInstance::PlayGunshotSoundAtLocation(const FVector& Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSoundCue, Location, FRotator::ZeroRotator);
}

void UAGPGameInstance::PlayGunshotSound2D()
{
	UGameplayStatics::PlaySound2D(GetWorld(), GunshotSoundCue);
}

void UAGPGameInstance::RespawnEnemy(AController* Controller)
{
	if (Controller)
	{
		if (AEnemyCharacter* CurrentEnemyCharacter = Cast<AEnemyCharacter>(Controller->GetPawn()))
		{
			Controller->Destroy();
			CurrentEnemyCharacter->Destroy();
			for (TActorIterator<APlayerStart> StartSpot(GetWorld()); StartSpot; ++StartSpot)
			{
				if (StartSpot->PlayerStartTag == FName("EnemyStart"))
				{
					AEnemyCharacter* NewEnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyCharacterClass.Get(), StartSpot->GetActorLocation(), FRotator::ZeroRotator);
				}
			}
		}
	}
}
