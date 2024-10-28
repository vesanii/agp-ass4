// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

void AHealthPickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Character healed!"));
		Player->HealCharacter(HealValue);
		Destroy();
	}
}
