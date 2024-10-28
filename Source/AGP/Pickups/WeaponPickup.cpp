// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"
#include <AGP/Characters/BaseCharacter.h>
#include <Net/UnrealNetwork.h>

void AWeaponPickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap event occured in WeaponPickup"));
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player != nullptr)
	{
		//if (!Player->HasWeapon())
		//{
			Player->EquipWeapon(true, WeaponStats);
			Destroy();
		//}
	}
}

AWeaponPickup::AWeaponPickup()
{
	bReplicates = true;
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority)
	{
		GenerateWeaponPickup();
	}
	UpdateWeaponPickupMaterial();
}

void AWeaponPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponPickup, WeaponRarity);
	DOREPLIFETIME(AWeaponPickup, WeaponStats);
}

void AWeaponPickup::GenerateWeaponPickup()
{
	GenerateRarity();
	TArray<bool> GoodStats{};

	switch (WeaponRarity)
	{
		case EWeaponRarity::Legendary:
			GoodStats = StatGenerator(4, 5);
			break;
		case EWeaponRarity::Master:
			GoodStats = StatGenerator(3, 5);
			break;
		case EWeaponRarity::Rare:
			GoodStats = StatGenerator(2, 5);
			break;
		case EWeaponRarity::Common:
			GoodStats = StatGenerator(0, 5);
			break;
	}
	WeaponStats.Accuracy = GoodStats[0] ? FMath::RandRange(0.98f, 1.0f) : FMath::RandRange(0.9f, 0.98f);
	WeaponStats.FireRate = GoodStats[1] ? FMath::RandRange(0.05f, 0.2f) : FMath::RandRange(0.2f, 1.0f);
	WeaponStats.BaseDamage = GoodStats[2] ? FMath::RandRange(15.0f, 30.0f) : FMath::RandRange(5.0f, 15.0f);
	WeaponStats.MagazineSize = GoodStats[3] ? FMath::RandRange(20, 100) : FMath::RandRange(1, 19);
	WeaponStats.ReloadTime = GoodStats[4] ? FMath::RandRange(0.1f, 1.0f) : FMath::RandRange(1.0f, 4.0f);
}

void AWeaponPickup::GenerateRarity()
{
	float Rarity = FMath::RandRange(0.0f, 1.0f);
	if (Rarity <= 0.05f)
	{
		WeaponRarity = EWeaponRarity::Legendary;
	}
	else if (Rarity <= 0.15f && Rarity > 0.05f)
	{
		WeaponRarity = EWeaponRarity::Master;
	}
	else if (Rarity <= 0.3f && Rarity > 0.15f)
	{
		WeaponRarity = EWeaponRarity::Rare;
	}
	else if (Rarity >= 0.5f && Rarity > 0.3f)
	{
		WeaponRarity = EWeaponRarity::Common;
	}
}

TArray<bool> AWeaponPickup::StatGenerator(int32 NumOfGoodStats, int32 TotalNumOfStats)
{
	TArray<bool> GoodStats{};
	for (int32 i = 0; i < TotalNumOfStats; ++i)
	{
		GoodStats.Add(i < NumOfGoodStats ? true : false); 
	}

	for (int32 i = 0; i < TotalNumOfStats; ++i)
	{
		int32 IndexToShuffle = FMath::RandRange(0, GoodStats.Num() - 1);
		GoodStats.Swap(i, IndexToShuffle);
	}

	return GoodStats;
}
