// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include <Net/UnrealNetwork.h>

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BulletStartPosition = CreateDefaultSubobject<USceneComponent>("Bullet");
	BulletStartPosition->SetupAttachment(GetRootComponent());
	CharacterHealth = CreateDefaultSubobject<UHealthComponent>("Health");
}

bool ABaseCharacter::HasWeapon()
{
	if (WeaponComponent)
	{
		return true;
	}
	return false;
}

void ABaseCharacter::EquipWeapon(bool bEquipWeapon, const FWeaponStats& WeaponStats)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		EquipWeaponImplementation(bEquipWeapon, WeaponStats);
		MulticastEquipWeapon(bEquipWeapon);
	}
	UE_LOG(LogTemp, Display, TEXT("Accuracy: %f"), WeaponStats.Accuracy);
	UE_LOG(LogTemp, Display, TEXT("FireRate: %f"), WeaponStats.FireRate);
	UE_LOG(LogTemp, Display, TEXT("BaseDamage: %f"), WeaponStats.BaseDamage);
	UE_LOG(LogTemp, Display, TEXT("MagazineSize: %i"), WeaponStats.MagazineSize);
	UE_LOG(LogTemp, Display, TEXT("ReloadTime: %f"), WeaponStats.ReloadTime);
}

void ABaseCharacter::HealCharacter(float HealingAmount)
{
	CharacterHealth->ApplyHealing(HealingAmount);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Fire(const FVector& FireAtLocation)
{
	if (HasWeapon())
	{
		WeaponComponent->Fire(BulletStartPosition->GetComponentLocation(), FireAtLocation);
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, WeaponComponent);
}

void ABaseCharacter::ChangeState(UBaseState* NewState)
{
}

bool ABaseCharacter::IsStunned()
{
	return CharacterHealth->IsStunned();
}

void ABaseCharacter::ResetStun()
{
	CharacterHealth->ResetStun();
}

void ABaseCharacter::EquipWeaponImplementation(bool bEquipWeapon, const FWeaponStats& WeaponStats)
{
	if (bEquipWeapon && !HasWeapon())
	{
		WeaponComponent = NewObject<UWeaponComponent>(this);
		WeaponComponent->RegisterComponent();
		//UE_LOG(LogTemp, Warning, TEXT("Character has equipped weapon"));
	}
	else if (bEquipWeapon && HasWeapon())
	{
		WeaponComponent->UnregisterComponent();
		WeaponComponent = nullptr;
		WeaponComponent = NewObject<UWeaponComponent>(this);
		WeaponComponent->RegisterComponent();
		//UE_LOG(LogTemp, Warning, TEXT("Character has equipped new weapon"));
	}
	else if (!bEquipWeapon && HasWeapon())
	{
		WeaponComponent->UnregisterComponent();
		WeaponComponent = nullptr;
		//UE_LOG(LogTemp, Warning, TEXT("Character has unequipped weapon"));
	}
	if (HasWeapon())
	{
		WeaponComponent->SetWeaponStats(WeaponStats);
	}
}

void ABaseCharacter::MulticastEquipWeapon_Implementation(bool bEquipWeapon, const FWeaponStats& WeaponStats)
{
	//EquipWeaponImplementation(bEquipWeapon);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

