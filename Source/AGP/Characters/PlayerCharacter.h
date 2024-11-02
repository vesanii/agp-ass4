// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UPlayerCharacterHUD;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

USTRUCT(BlueprintType)
struct FScores
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 HighestKills = 0;
	UPROPERTY()
	int32 CurrentKills = 0;
	UPROPERTY()
	int32 Deaths = 0;
};

UCLASS()
class AGP_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void UpdateHealthBar(float HealthPercent);
	void UpdateAmmoUI(int32 RoundsRemaining, int32 MagazineSize);
	UFUNCTION()
	void UpdateScoreBoard();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerDeath();
	UFUNCTION(BlueprintImplementableEvent)
	void ChooseCharacterMesh();
	void DrawUI();
	/*
	Method to update player scores. If bUpdateKills is true, then PlayerScores.Kills
	is increased by 1. Else PlayerScores.Deaths is increased by 1.
	Also updates PlayerScores.HighestKills to match the current highest kill count.
	*/
	void UpdatePlayerScores(bool bUpdateKills);
	FScores GetPlayerScores();
	void SetPlayerScores(const FScores& Scores);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* ReloadAction;
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	float LookSensitivity = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerCharacterHUD> PlayerHUDClass;
	UPROPERTY()
	UPlayerCharacterHUD* PlayerHUD;
	UPROPERTY(ReplicatedUsing = UpdateScoreBoard);
	FScores PlayerScores;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void FireWeapon(const FInputActionValue& Value);

	void UpdatePlayerScoresImplementation(bool bUpdateKills);
	UFUNCTION(Server, Reliable)
	void ServerUpdatePlayerScores(bool bUpdateKills);
};
