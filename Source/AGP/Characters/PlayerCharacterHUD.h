// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacterHUD.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UPlayerCharacterHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthBar(float HealthPercent);
	void SetAmmoText(int32 RoundsRemaining, int32 MagazineSize);
	void SetCurrentKillsText(int32 KillCount);
	void SetHighestKillText(int32 HighestKillCount);
	void SetLeaderBoardText(int32 HighestKillCount, int32 KillCount, int32 DeathCount);

protected:

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* AmmoText;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HighestKills;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CurrentKills;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Deaths;
};
