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


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UHealthComponent::OnDeath()
{
	bIsDead = true;
	UE_LOG(LogTemp, Warning, TEXT("The character has died"));
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UHealthComponent::IsDead()
{
	return bIsDead;
}

bool UHealthComponent::IsStunned()
{
	return bIsStunned;
}

float UHealthComponent::GetCurrentHealth()
{
	return CurrentHealth;
}

float UHealthComponent::GetCurrentHealthPercentage()
{
	return CurrentHealth/MaxHealth;
}

void UHealthComponent::ApplyDamage(float DamageAmount)
{
	if (bIsDead)
	{
		return;
	}
	//if 15 or more damage is taken at once at full health, character is stunned
	if (CurrentHealth >= 100.0f && DamageAmount >= 15.0f)
	{
		bIsStunned = true;
	}
	CurrentHealth -= DamageAmount;
	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
		CurrentHealth = 0.0f;
	}
}

void UHealthComponent::ApplyHealing(float HealingAmount)
{
	if (bIsDead)
	{
		return;
	}
	CurrentHealth += HealingAmount;
	if (CurrentHealth > 100.0f)
	{
		CurrentHealth = 100.0f;
	}
}

void UHealthComponent::ResetStun()
{
	bIsStunned = false;
}

