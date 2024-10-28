// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Pickups/WeaponPickup.h"
#include "Pickups/HealthPickup.h"
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
	UPROPERTY(EditDefaultsOnly, Category = "Pickup Classes")
	TSubclassOf<AHealthPickup> HealthPickupClass;
public:
	UClass* GetWeaponPickupClass() const;
	UClass* GetHealthPickupClass() const;
};
