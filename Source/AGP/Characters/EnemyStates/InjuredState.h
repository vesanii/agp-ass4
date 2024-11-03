// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGP/StateMachine/BaseState.h"
#include "InjuredState.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UInjuredState : public UBaseState
{
	GENERATED_BODY()

public:
	void Entry(AEnemyCharacter* Owner) override;
	void Update(AEnemyCharacter* Owner, float DeltaTime) override;
	void Exit(AEnemyCharacter* Owner) override;
	
};
