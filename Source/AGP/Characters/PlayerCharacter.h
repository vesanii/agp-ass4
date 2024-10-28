// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AGP_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly)
	UInputAction* FireAction;
	UPROPERTY(EditdefaultsOnly)
	UInputAction* ReloadAction;
	UPROPERTY(EditDefaultsOnly)
	float Sensitivity = 1.0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void FireWeapon(const FInputActionValue& Value);
	void ReloadWeapon(const FInputActionValue& Value);
};

