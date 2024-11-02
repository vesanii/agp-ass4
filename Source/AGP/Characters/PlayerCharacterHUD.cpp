// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerCharacterHUD::SetHealthBar(float HealthPercent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercent);
	}
}

void UPlayerCharacterHUD::SetAmmoText(int32 RoundsRemaining, int32 MagazineSize)
{
	if (AmmoText)
	{
		AmmoText->SetText(FText::FromString(FString::FromInt(RoundsRemaining) + " / " + FString::FromInt(MagazineSize)));
	}
}

void UPlayerCharacterHUD::SetCurrentKillsText(int32 KillCount)
{
	if (CurrentKills)
	{
		CurrentKills->SetText(FText::FromString(FString::FromInt(KillCount)));
	}
}

void UPlayerCharacterHUD::SetHighestKillText(int32 HighestKillCount)
{
	if (HighestKills)
	{
		HighestKills->SetText(FText::FromString(FString::FromInt(HighestKillCount)));
	}
}

void UPlayerCharacterHUD::SetLeaderBoardText(int32 HighestKillCount, int32 KillCount, int32 DeathCount)
{
	if (HighestKills)
	{
		HighestKills->SetText(FText::FromString(FString::FromInt(HighestKillCount)));
	}
	if (CurrentKills)
	{
		CurrentKills->SetText(FText::FromString(FString::FromInt(KillCount)));
	}
	if (Deaths)
	{
		Deaths->SetText(FText::FromString(FString::FromInt(DeathCount)));
	}
}
