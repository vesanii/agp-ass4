// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

bool UHealthComponent::IsDead()
{
	return bIsDead;
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetCurrentHealthPercentage() const
{
	return CurrentHealth / MaxHealth;
}

void UHealthComponent::UpdateHealing(float DeltaTime)
{
	if (bIsDead) return;

	TimeSinceLastDamage += DeltaTime;

	// Start healing if no damage has been taken for 10 seconds
	if (TimeSinceLastDamage >= 10.0f)
	{
		bIsHealing = true;
	}

	// If healing is active, restore health at the healing rate
	if (bIsHealing)
	{
		float HealthToRestore = MaxHealth * HealingRate * DeltaTime;
		ApplyHealing(HealthToRestore);
	}
}

void UHealthComponent::ApplyDamage(float DamageAmount)
{
	if (bIsDead) return;

	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
		CurrentHealth = 0.0f;
		return;
	}

	TimeSinceLastDamage = 0.0f;
	bIsHealing = false;

	// Broadcast the damage event
	OnTakeDamage.Broadcast();
}

void UHealthComponent::ApplyHealing(float HealingAmount)
{
    if (bIsDead || CurrentHealth >= 100.0f) return;

    CurrentHealth += HealingAmount;

    if (CurrentHealth > 100.0f)
    {
        CurrentHealth = 100.0f;
    }
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = MaxHealth;
}


void UHealthComponent::OnDeath()
{
	UE_LOG(LogTemp, Display, TEXT("The character has died."))
	bIsDead = true;
}
void UHealthComponent::StartImmediateHealing()
{
	// Set bIsHealing to true immediately, no delay
	bIsHealing = true;
	TimeSinceLastDamage = 10.0f; // Set to 10 to bypass the normal delay in UpdateHealing
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

