// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Pickups/WeaponPickup.h"
#include "Pickups/HealthPickup.h"
#include "NiagaraFunctionLibrary.h"
#include <NiagaraSystem.h>
#include <Engine.h>
#include "AGPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UAGPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Pickup Classes")
	TSubclassOf<AWeaponPickup> WeaponPickupClass;
	UPROPERTY(EditDefaultsOnly, Category="Pickup Classes")
	TSubclassOf<AHealthPickup> HealthPickupClass;
	UPROPERTY(EditDefaultsOnly, Category="Particle Systems")
	UNiagaraSystem* GroundHitParticles;
	UPROPERTY(EditAnywhere, Category = "Particle Systems")
	UNiagaraSystem* BloodSpatterEffect;
	UPROPERTY(EditDefaultsOnly)
	USoundCue* GunshotSoundCue;
public:
	UClass* GetWeaponPickupClass() const;
	UClass* GetHealthPickupClass() const;
	void SpawnGroundHitParticles(const FVector& SpawnLocation);
	void SpawnBloodSpatterParticles(const FVector& SpawnLocation);
	void PlayGunshotSoundAtLocation(const FVector& Location);
	void PlayGunshotSound2D();
};
