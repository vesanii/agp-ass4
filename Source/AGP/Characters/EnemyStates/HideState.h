// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGP/StateMachine/BaseState.h"
#include "HideState.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UHideState : public UBaseState
{
	GENERATED_BODY()
	
public:
	void Entry(AEnemyCharacter* Owner) override;
	void Update(AEnemyCharacter* Owner, float DeltaTime) override;
	void Exit(AEnemyCharacter* Owner) override;

private:
	bool bReachedCover = false;
	float CooldownTime = 2.0f; 
	float ElapsedTime = 0.0f;
	float HealingRate = 5.0f;
};
