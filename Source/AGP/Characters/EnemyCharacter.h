// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"
#include "HealthComponent.h"
#include "../Pickups/WeaponPickup.h"
#include "../Pickups/HealthPickup.h"
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

/**
 * A class representing the logic for an AI controlled enemy character. 
 */
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

	/**
	 * A function bound to the UPawnSensingComponent's OnSeePawn event. This will set the SensedCharacter variable
	 * if the pawn that was sensed was of type APlayerCharacter.
	 * @param SensedActor The pawn that was sensed by the UPawnSensingComponent.
	 */
	UFUNCTION()
	void OnSensedPawn(APawn* SensedActor);
	/**
	 * Will update the SensedCharacter variable based on whether the UPawnSensingComponent has a line of sight to the
	 * Player Character or not. This may cause the SensedCharacter variable to become a nullptr so be careful when using
	 * the SensedCharacter variable.
	 */
	void UpdateSight();

	//helper functions to use within states to perform actions
	float GetHealth();
	void CreateRandomPath();
	void CreatePathTo(AActor* Target);
	void CreatePathTo(FVector Location);
	void CreatePathAwayFrom(APawn* Target);
	void EmptyCurrentPath();
	void Investigate(const float& DeltaTime);
	//main funtion of the FSM, used in the update function of states when paramters for a state change are met
	void ChangeState(UBaseState* NewState) override;
	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	/**
	 * A pointer to the Pathfinding Subsystem.
	 */
	UPROPERTY()
	UPathfindingSubsystem* PathfindingSubsystem;

	/**
	 * A pointer to the PawnSensingComponent attached to this enemy character.
	 */
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;

	/**
	 * A pointer to a PlayerCharacter that can be seen by this enemy character. If this is nullptr then the enemy cannot
	 * see any PlayerCharacter.
	 */
	UPROPERTY()
	TWeakObjectPtr<APlayerCharacter> SensedCharacter = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AWeaponPickup> SensedWeapon = nullptr;
	UPROPERTY()
	TWeakObjectPtr<AHealthPickup> SensedHealUp = nullptr;
	FVector LastKnownCharacterLocation;

	/**
	 * An array of vectors representing the current path that the agent is traversing along.
	 */
	UPROPERTY(VisibleAnywhere)
	TArray<FVector> CurrentPath;

	/**
	 * Some arbitrary error value for determining how close is close enough before moving onto the next step in the path.
	 */
	UPROPERTY(EditAnywhere)
	float PathfindingError = 100.0f; // 100 cm from target by default.

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FString GetStateName() const;


private:

	//helper functions to find the nearest pickup
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
};
