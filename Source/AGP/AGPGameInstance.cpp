// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPGameInstance.h"

UClass* UAGPGameInstance::GetWeaponPickupClass() const
{
	return WeaponPickupClass.Get();
}

UClass* UAGPGameInstance::GetHealthPickupClass() const
{
	return HealthPickupClass.Get();
}

void UAGPGameInstance::SpawnGroundHitParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundHitParticles, SpawnLocation);
}

void UAGPGameInstance::SpawnBloodSpatterParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSpatterEffect, SpawnLocation);
}

void UAGPGameInstance::PlayGunshotSoundAtLocation(const FVector& Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSoundCue, Location, FRotator::ZeroRotator);
}

void UAGPGameInstance::PlayGunshotSound2D()
{
	UGameplayStatics::PlaySound2D(GetWorld(), GunshotSoundCue);
}
