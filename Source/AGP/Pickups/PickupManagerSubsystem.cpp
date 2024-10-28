// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"
#include <AGP/Pathfinding/PathfindingSubsystem.h>
#include <AGP/AGPGameInstance.h>

TStatId UPickupManagerSubsystem::GetStatId() const
{
	return TStatId();
}

void UPickupManagerSubsystem::Tick(float DeltaTime)
{
	if (GetWorld()->GetNetMode() >= NM_Client)
	{
		return;
	}
	if (PossibleSpawnLocations.IsEmpty())
	{
		PopulateSpawnLocations();
	}
	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= PickupSpawnRate)
	{
		TimeSinceLastSpawn = 0.0f;
		SpawnWeaponPickup();
		SpawnHealthPickup();
	}
}

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	PossibleSpawnLocations.Empty();
	PossibleSpawnLocations = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->GetWaypointPositions();
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	if (PossibleSpawnLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to spawn weapon pickup")); //debugging for if there are no navigation nodes in the world
		return;
	}
	if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>()) // if the world is loaded with the custom game instance
	{
		FVector SpawnPosition = PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num() - 1)]; // find a random position on the map
		SpawnPosition.Z += 50.0f; // raise its position so it's not stuck in the floor
		AWeaponPickup* Pickup = GetWorld()->SpawnActor<AWeaponPickup>(GameInstance->GetWeaponPickupClass(), SpawnPosition, FRotator::ZeroRotator); // spawn a bp_weaponpickup at the desired location
		//UE_LOG(LogTemp, Warning, TEXT("Weapon Pickup Spawned"));
	}
}

void UPickupManagerSubsystem::SpawnHealthPickup()
{
	if (PossibleSpawnLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to spawn health pickup")); //debugging for if there are no navigation nodes in the world
		return;
	}
	if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>()) // if the world is loaded with the custom game instance
	{
		FVector SpawnPosition = PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num() - 1)]; // find a random position on the map
		SpawnPosition.Z += 50.0f; // raise its position so it's not stuck in the floor
		AHealthPickup* Pickup = GetWorld()->SpawnActor<AHealthPickup>(GameInstance->GetHealthPickupClass(), SpawnPosition, FRotator::ZeroRotator); // spawn a bp_healthpickup at the desired location
		//UE_LOG(LogTemp, Warning, TEXT("Health Pickup Spawned"));
	}
}
