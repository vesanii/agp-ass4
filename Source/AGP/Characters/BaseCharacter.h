// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Engine/HitResult.h"
#include "AGP/StateMachine/BaseState.h"
#include "AGP/Components/HealthComponent.h"
#include "AGP/Components/WeaponComponent.h"
#include "BaseCharacter.generated.h"

UCLASS()
class AGP_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();
	UFUNCTION(BlueprintCallable)
	bool HasWeapon();
	void EquipWeapon(bool bEquipWeapon, const FWeaponStats& WeaponStats);
	UFUNCTION(BlueprintCallable)
	bool HasDied() { return CharacterHealth->IsDead(); };
	void HealCharacter(float HealingAmount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(Replicated)
	UWeaponComponent* WeaponComponent = nullptr;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BulletStartPosition;
	UHealthComponent* CharacterHealth;

	void Fire(const FVector& FireAtLocation);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void ChangeState(UBaseState* NewState);
	bool IsStunned();
	void ResetStun();
	

private:
	void EquipWeaponImplementation(bool bEquipWeapon, const FWeaponStats& WeaponStats = FWeaponStats());
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEquipWeapon(bool bEquipWeapon, const FWeaponStats& WeaponStats = FWeaponStats());


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
