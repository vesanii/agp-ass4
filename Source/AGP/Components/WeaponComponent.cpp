// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "AGP/Characters/BaseCharacter.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	RoundsRemainingInMagazine = WeaponStats.MagazineSize;
	// ...
	
}


bool UWeaponComponent::FireImplementation(const FVector& BulletStart, const FVector& FireAtLocation, FVector& OutHitLocation)
{
	if (TimeSinceLastShot < WeaponStats.FireRate || IsMagazineEmpty())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player failed to fire"));
		return false;
	}

	// Creates a random direction vector.
	FVector RandomFireAt = FMath::VRand();
	float CurrentShotDistance = FVector::Distance(BulletStart, FireAtLocation);
	// Makes that random direction vector the same length as the current shot between the bullet start and fire at location.
	RandomFireAt *= CurrentShotDistance;
	// Offsets the direction vector by the Bullet Start position making this RandomFireAt now contain a random position
	// somewhere on the surface a sphere surrounding the bullet start position. The original FireAtLocation is also on
	// the surface of this same sphere.
	RandomFireAt += BulletStart;
	// Now we just need to blend between these two positions based on the accuracy value.
	FVector AccuracyAdjustedFireAt = FMath::Lerp(RandomFireAt, FireAtLocation, WeaponStats.Accuracy);

	FHitResult Outhit;
	FCollisionQueryParams CollisionParams;

	CollisionParams.AddIgnoredActor(this->GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(Outhit, BulletStart, FireAtLocation, ECC_Pawn, CollisionParams);

	if (bHit)
	{
		OutHitLocation = Outhit.ImpactPoint;
		ABaseCharacter* Target = Cast<ABaseCharacter>(Outhit.GetActor());
		if (Target)
		{
			DrawDebugLine(GetWorld(), BulletStart, Outhit.Location, FColor::Green, false, 1.0f);
			Target->GetComponentByClass<UHealthComponent>()->ApplyDamage(WeaponStats.BaseDamage);
		}
		else
		{
			DrawDebugLine(GetWorld(), BulletStart, Outhit.Location, FColor::Orange, false, 1.0f);
		}
	}
	else
	{
		OutHitLocation = AccuracyAdjustedFireAt;
		DrawDebugLine(GetWorld(), BulletStart, FireAtLocation, FColor::Red, false, 1.0f);
	}
	TimeSinceLastShot = 0.0f;
	RoundsRemainingInMagazine--;
	return true;
}

void UWeaponComponent::FireVisualImplementation(const FVector& BulletStart, const FVector& HitLocation)
{
	DrawDebugLine(GetWorld(), BulletStart, HitLocation, FColor::Blue, false, 1.0f);
}

void UWeaponComponent::ServerFire_Implementation(const FVector& BulletStart, const FVector& FireAtLocation)
{
	FVector Outhit{};
	if (FireImplementation(BulletStart, FireAtLocation, Outhit))
	{
		MulticastFire(BulletStart, Outhit);
	}
}

void UWeaponComponent::MulticastFire_Implementation(const FVector& BulletStart, const FVector& HitLocation)
{
	FireVisualImplementation(BulletStart, HitLocation);
}

void UWeaponComponent::CompleteReload()
{
	RoundsRemainingInMagazine = WeaponStats.MagazineSize;
}

void UWeaponComponent::ReloadMagazineImplementation()
{
	if (bIsReloading) { return; }
	bIsReloading = true;
}

void UWeaponComponent::ServerReloadMagazine_Implementation()
{
	ReloadMagazineImplementation();
}

// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TimeSinceLastShot += DeltaTime;
	
	if (bIsReloading)
	{
		TimeSinceReload += DeltaTime;
		if (TimeSinceReload >= WeaponStats.ReloadTime)
		{
			bIsReloading = false;
			CompleteReload();
			TimeSinceReload = 0.0f;
		}
	}

	// ...
}

void UWeaponComponent::Fire(const FVector& BulletStart, const FVector& FireAtLocation)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		FVector Outhit{};
		if (FireImplementation(BulletStart, FireAtLocation, Outhit)) {
			MulticastFire(BulletStart, Outhit);
		}
	}
	else
	{
		ServerFire(BulletStart, FireAtLocation);
	}
}

void UWeaponComponent::SetWeaponStats(const FWeaponStats& NewWeaponStats)
{
	this->WeaponStats = NewWeaponStats;
	RoundsRemainingInMagazine = WeaponStats.MagazineSize;
}

bool UWeaponComponent::IsMagazineEmpty()
{
	return RoundsRemainingInMagazine <= 0;
}

void UWeaponComponent::ReloadMagazine()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ReloadMagazineImplementation();
	}
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		ServerReloadMagazine();
	}
}

