// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	bool IsDead();
	bool IsStunned();
	float GetCurrentHealth() const;
	float GetCurrentHealthPercentage() const;
	void ApplyDamage(float DamageAmount, ABaseCharacter* DamageSource);
	void ApplyHealing(float HealingAmount);
	void ResetHealth();
	void ResetStun();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	float MaxHealth = 100.0f;
	UPROPERTY(ReplicatedUsing=UpdateHealthBar)
	float CurrentHealth;
	bool bIsDead = false;
	bool bIsStunned = false;
	ABaseCharacter* LastDamageSource = nullptr;

	void OnDeath();
	UFUNCTION()
	void UpdateHealthBar();
	UFUNCTION()
	void UpdateScoreBoard();

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
