// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PickupManagerSubsystem.generated.h"

class AWeaponPickup;
class AHealthPickup;
/**
 * 
 */
UCLASS()
class AGP_API UPickupManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:

	virtual TStatId GetStatId() const override
	{
		return TStatId();
	}
	void DestroyWeaponPickup(AWeaponPickup* PickupToDestroy);
	void DestroyHealthPickup(AHealthPickup* PickupToDestroy);

protected:
	
	/**
	 * The world locations of all possible locations that a pickup can spawn.
	 */
	TArray<TPair<TWeakObjectPtr<AWeaponPickup>, FVector>> PossibleWeaponSpawnLocations;
	TArray<TPair<TWeakObjectPtr<AHealthPickup>, FVector>> PossibleHealthSpawnLocations;
	float WeaponPickupSpawnRate = 5.0f;
	float HealthPickupSpawnRate = 8.0f;
	float TimeSinceLastWeaponSpawn = 0.0f;
	float TimeSinceLastHealthSpawn = 0.0f;
	
	virtual void Tick(float DeltaTime) override;

private:

	void SpawnWeaponPickup();
	void SpawnHealthPickup();
	void PopulateSpawnLocations();
	
};
