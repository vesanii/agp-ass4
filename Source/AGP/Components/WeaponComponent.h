// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle,
	Pistol
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()
public:
	EWeaponType WeaponType = EWeaponType::Rifle;
	float Accuracy = 1.0f;
	float FireRate = 0.2f;
	float BaseDamage = 10.0f;
	int32 MagazineSize = 5;
	float ReloadTime = 1.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	FWeaponStats WeaponStats;
	int32 RoundsRemainingInMagazine;
	float TimeSinceLastShot = 0.0f;
	float TimeSinceReload = 0.0f;
	bool bIsReloading;
	
private:
	bool FireImplementation(const FVector& BulletStart, const FVector& FireAtLocation, FVector& OutHitLocation);
	void FireVisualImplementation(const FVector& BulletStart, const FVector& HitLocation);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastFire(const FVector& BulletStart, const FVector& HitLocation);
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector& BulletStart, const FVector& FireAtLocation);
	void CompleteReload();
	void ReloadMagazineImplementation();
	UFUNCTION(Server, Reliable)
	void ServerReloadMagazine();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Fire(const FVector& BulletStart, const FVector& FireAtLocation);
	
	void SetWeaponStats(const FWeaponStats& NewWeaponStats);
	bool IsMagazineEmpty();
	void ReloadMagazine();
};
