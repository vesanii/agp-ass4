// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"
#include "Perception/PawnSensingComponent.h"
#include "PlayerCharacter.h"
#include "AGP/Pickups/WeaponPickup.h"
#include "AGP/Pickups/HealthPickup.h"
#include "EnemyStates/PatrolState.h"
#include "EnemyStates/EngageState.h"
#include "EnemyStates/EvadeState.h"
#include "EnemyStates/UnarmedState.h"
#include "EnemyStates/DeadState.h"
#include "EnemyStates/InvestigateState.h"
#include "EnemyStates/InjuredState.h"
#include "EnemyStates/HideState.h"
#include "EnemyStates/StunnedState.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class AGP_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveAlongPath();
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
	void UpdateSight();
	float GetHealth();
	//helper functions to use within states to perform actions
	void CreateRandomPath();
	void CreatePathTo(AActor* Target);
	void CreatePathTo(FVector Location);
	void CreatePathAwayFrom(APawn* Target);
	void EmptyCurrentPath();
	void Investigate(const float& DeltaTime);
	//main funtion of the FSM, used in the update function of states when paramters for a state change are met
	void ChangeState(UBaseState* NewState) override;

	UPROPERTY()
	UPathfindingSubsystem* PathfindingSubsystem;
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> CurrentPath;
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;
	UPROPERTY()
	APlayerCharacter* SensedCharacter = nullptr;
	UPROPERTY()
	AWeaponPickup* SensedWeapon = nullptr;
	UPROPERTY()
	AHealthPickup* SensedHealUp = nullptr;
	FVector LastKnownCharacterLocation;

private:
	//temporary functions for pickup sensing, to be replaced with ai perception in future
	void FindWeaponPickup();
	void FindHealthPickup();

	UPROPERTY()
	UBaseState* ActiveState;

	UPROPERTY()
	UPatrolState* PatrolState = nullptr;
	UPROPERTY()
	UEngageState* EngageState = nullptr;
	UPROPERTY()
	UEvadeState* EvadeState = nullptr;
	UPROPERTY()
	UUnarmedState* UnarmedState = nullptr;
	UPROPERTY()
	UDeadState* DeadState = nullptr;
	UPROPERTY()
	UInvestigateState* InvestigateState = nullptr;
	UPROPERTY()
	UInjuredState* InjuredState = nullptr;
	UPROPERTY()
	UHideState* HideState = nullptr;
	UPROPERTY()
	UStunnedState* StunnedState = nullptr;

	friend class UPatrolState;
	friend class UEngageState;
	friend class UEvadeState;
	friend class UUnarmedState;
	friend class UDeadState;
	friend class UInvestigateState;
	friend class UInjuredState;
	friend class UHideState;
	friend class UStunnedState;

	//helper function to instantiate state objects at begin play
	void InstantiateStates();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};