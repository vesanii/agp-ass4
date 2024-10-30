// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "WeaponPickup.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UPickupManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We don't want this pickup manager to do any spawning if it isn't
	// on the server.
	// A value < NM_Client is any type of server. So if it is >=
	// to NM_Client or == NM_Client then we know it is the client
	// and we don't want to spawn.
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
		SpawnWeaponPickup();
		TimeSinceLastSpawn = 0.0f;
	}
}

void UPickupManagerSubsystem::DestroyWeaponPickup(TPair<AWeaponPickup*, FVector> PickupToDestroy)
{
	if (GetWorld()->GetNetMode() >= NM_Client)
	{
		return;
	}
	int32 index = PossibleSpawnLocations.Find(PickupToDestroy);
	if (index < PossibleSpawnLocations.Num())
	{
		TPair<AWeaponPickup*, FVector>* Pair = &PossibleSpawnLocations[index];
		if (Pair->Key)
		{
			Pair->Key->Destroy();
		}
	}
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
		TPair<AWeaponPickup*, FVector>* SpawnPosition = &PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num() - 1)]; // find a random position on the map
		if (SpawnPosition->Key)
		{
			SpawnPosition->Key->Destroy();
			SpawnPosition->Key = GetWorld()->SpawnActor<AWeaponPickup>(GameInstance->GetWeaponPickupClass(), SpawnPosition->Value, FRotator::ZeroRotator);
			//UE_LOG(LogTemp, Warning, TEXT("Weapon Pickup Replaced"));
		}
		else
		{
			SpawnPosition->Value.Z += 50.0f; // raise its position so it's not stuck in the floor
			SpawnPosition->Key = GetWorld()->SpawnActor<AWeaponPickup>(GameInstance->GetWeaponPickupClass(), SpawnPosition->Value, FRotator::ZeroRotator); // spawn a bp_weaponpickup at the desired location
			//UE_LOG(LogTemp, Warning, TEXT("Weapon Pickup Spawned"));
		}
	}
}

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	PossibleSpawnLocations.Empty();
	TArray<FVector> Waypoints = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->GetWaypointPositions();
	for (FVector Waypoint : Waypoints)
	{
		PossibleSpawnLocations.Push(TPair<AWeaponPickup*, FVector>(nullptr, Waypoint));
		//empty pointer means a weapon hasn't been spawned here yet
	}
}
