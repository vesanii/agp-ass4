// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamage);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	void StartImmediateHealing();
	bool IsDead();
	float GetCurrentHealth() const;
	float GetCurrentHealthPercentage() const;
	void ApplyDamage(float DamageAmount);
	void ApplyHealing(float HealingAmount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float MaxHealth = 100.0f;
	float CurrentHealth;
	bool bIsDead = false;

	void OnDeath();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	float TimeSinceLastDamage = 0.0f;
	bool bIsHealing = false;
	float HealingRate = 0.05f; 

public:
	void UpdateHealing(float DeltaTime);
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTakeDamage OnTakeDamage;
};
