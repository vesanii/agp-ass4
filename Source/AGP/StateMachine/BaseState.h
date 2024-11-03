// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseState.generated.h"

/**
 * 
 */
class AEnemyCharacter;
class APlayerCharacter;

UCLASS()
class AGP_API UBaseState : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Entry(AEnemyCharacter* Owner);
	virtual void Update(AEnemyCharacter* Owner, float DeltaTime);
	virtual void Exit(AEnemyCharacter* Owner);

	virtual void Entry(APlayerCharacter* Owner);
	virtual void Update(APlayerCharacter* Owner, float DeltaTime);
	virtual void Exit(APlayerCharacter* Owner);
};
