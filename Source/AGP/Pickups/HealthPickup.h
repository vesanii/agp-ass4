// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGP/Pickups/PickupBase.h"
#include "AGP/Characters/BaseCharacter.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API AHealthPickup : public APickupBase
{
	GENERATED_BODY()
	
	virtual void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	float HealValue = 50.0f;
};
