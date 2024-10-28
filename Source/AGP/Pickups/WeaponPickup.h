// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "AGP/Components/WeaponComponent.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponRarity : uint8
{
	Common,
	Rare,
	Master,
	Legendary
};
UCLASS()
class AGP_API AWeaponPickup : public APickupBase
{
	GENERATED_BODY()
	
	virtual void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	AWeaponPickup();

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWeaponPickupMaterial();
	UPROPERTY(BlueprintReadOnly, Replicated)
	EWeaponRarity WeaponRarity = EWeaponRarity::Common;
	UPROPERTY(Replicated)
	FWeaponStats WeaponStats;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
private:
	void GenerateWeaponPickup();
	void GenerateRarity();
	TArray<bool> StatGenerator(int32 NumOfGoodStats, int32 TotalNumOfStats);
};
