// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "WeaponPickup.h"
#include "HealthPickup.h"
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

	if (PossibleWeaponSpawnLocations.IsEmpty() || PossibleHealthSpawnLocations.IsEmpty())
	{
		PopulateSpawnLocations();
	}

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= PickupSpawnRate)
	{
		SpawnWeaponPickup();
		SpawnHealthPickup();
		TimeSinceLastSpawn = 0.0f;
	}
}

void UPickupManagerSubsystem::DestroyWeaponPickup(TPair<AWeaponPickup*, FVector> PickupToDestroy)
{
	if (GetWorld()->GetNetMode() >= NM_Client)
	{
		return;
	}
	int32 index = PossibleWeaponSpawnLocations.Find(PickupToDestroy);
	if (index < PossibleWeaponSpawnLocations.Num() && index >= 0)
	{
		TPair<AWeaponPickup*, FVector>* Pair = &PossibleWeaponSpawnLocations[index];
		if (Pair->Key)
		{
			Pair->Key->Destroy();
		}
	}
}

void UPickupManagerSubsystem::DestroyHealthPickup(TPair<AHealthPickup*, FVector> PickupToDestroy)
{
	if (GetWorld()->GetNetMode() >= NM_Client)
	{
		return;
	}
	int32 index = PossibleHealthSpawnLocations.Find(PickupToDestroy);
	if (index < PossibleHealthSpawnLocations.Num() && index >= 0)
	{
		TPair<AHealthPickup*, FVector>* Pair = &PossibleHealthSpawnLocations[index];
		if (Pair->Key)
		{
			Pair->Key->Destroy();
		}
	}
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	if (PossibleWeaponSpawnLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to spawn weapon pickup")); //debugging for if there are no navigation nodes in the world
		return;
	}
	if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>()) // if the world is loaded with the custom game instance
	{
		int32 RandIndex = FMath::RandRange(0, PossibleWeaponSpawnLocations.Num() - 1);
		if (RandIndex < PossibleHealthSpawnLocations.Num())
		{
			TPair<AHealthPickup*, FVector>* PossibleSpawnPosition = &PossibleHealthSpawnLocations[RandIndex];
			if (PossibleSpawnPosition->Key)
			{
				UE_LOG(LogTemp, Display, TEXT("Weapon pickup failed to spawn on top of health pickup"));
				return;
			}
		}
		TPair<AWeaponPickup*, FVector>* SpawnPosition = &PossibleWeaponSpawnLocations[RandIndex];
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

void UPickupManagerSubsystem::SpawnHealthPickup()
{
	if (PossibleHealthSpawnLocations.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to spawn health pickup")); //debugging for if there are no navigation nodes in the world
		return;
	}
	if (const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>()) // if the world is loaded with the custom game instance
	{
		int32 RandIndex = FMath::RandRange(0, PossibleHealthSpawnLocations.Num() - 1);
		if (RandIndex < PossibleWeaponSpawnLocations.Num())
		{
			TPair<AWeaponPickup*, FVector>* PossibleSpawnPosition = &PossibleWeaponSpawnLocations[RandIndex];
			if (PossibleSpawnPosition->Key)
			{
				UE_LOG(LogTemp, Display, TEXT("Health pickup failed to spawn on top of weapon pickup"));
				return;
			}
		}
		TPair<AHealthPickup*, FVector>* SpawnPosition = &PossibleHealthSpawnLocations[RandIndex]; 
		if (SpawnPosition->Key)
		{
			SpawnPosition->Key->Destroy();
			SpawnPosition->Key = GetWorld()->SpawnActor<AHealthPickup>(GameInstance->GetHealthPickupClass(), SpawnPosition->Value, FRotator::ZeroRotator);
			//UE_LOG(LogTemp, Warning, TEXT("Health Pickup Replaced"));
		}
		else
		{
			SpawnPosition->Value.Z += 50.0f; // raise its position so it's not stuck in the floor
			SpawnPosition->Key = GetWorld()->SpawnActor<AHealthPickup>(GameInstance->GetHealthPickupClass(), SpawnPosition->Value, FRotator::ZeroRotator); // spawn a bp_healthpickup at the desired location
			//UE_LOG(LogTemp, Warning, TEXT("Health Pickup Spawned"));
		}
	}
}

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	PossibleWeaponSpawnLocations.Empty();
	PossibleHealthSpawnLocations.Empty();
	TArray<FVector> Waypoints = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->GetWaypointPositions();
	for (FVector Waypoint : Waypoints)
	{
		PossibleWeaponSpawnLocations.Push(TPair<AWeaponPickup*, FVector>(nullptr, Waypoint));
		PossibleHealthSpawnLocations.Push(TPair<AHealthPickup*, FVector>(nullptr, Waypoint));
		//empty pointer means a weapon hasn't been spawned here yet
	}
}
