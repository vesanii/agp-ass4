// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "../Pickups/PickupManagerSubsystem.h"

void AHealthPickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Character healed!"));
		Player->HealCharacter(HealValue);
		DestroyPickup();
	}
}

void AHealthPickup::DestroyPickup()
{
	if (UPickupManagerSubsystem* PickupManagerSubsystem = GetWorld()->GetSubsystem<UPickupManagerSubsystem>())
	{
		PickupManagerSubsystem->DestroyHealthPickup(TPair<AHealthPickup*, FVector>(this, this->GetActorLocation()));
	}
}
