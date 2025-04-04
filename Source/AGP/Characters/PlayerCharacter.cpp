// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthComponent.h"
#include "PlayerCharacterHUD.h"
#include "Kismet/KismetMathLibrary.h"
#include <Net/UnrealNetwork.h>
#include <AGP/MultiplayerGameMode.h>

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::UpdateHealthBar(float HealthPercent)
{
	if (PlayerHUD && IsLocallyControlled())
	{
		PlayerHUD->SetHealthBar(HealthPercent);
	}
}

void APlayerCharacter::UpdateAmmoUI(int32 RoundsRemaining, int32 MagazineSize)
{
	if (PlayerHUD && IsLocallyControlled())
	{
		PlayerHUD->SetAmmoText(RoundsRemaining, MagazineSize);
	}
}

void APlayerCharacter::UpdateScoreBoard()
{
	if (PlayerHUD && IsLocallyControlled())
	{
		PlayerHUD->SetLeaderBoardText(PlayerScores.HighestKills, PlayerScores.CurrentKills, PlayerScores.Deaths);
	}
}

void APlayerCharacter::DrawUI()
{
	if (IsLocallyControlled() && PlayerHUDClass)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
		{
			PlayerHUD = CreateWidget<UPlayerCharacterHUD>(PlayerController, PlayerHUDClass);
			if (PlayerHUD)
			{
				PlayerHUD->AddToPlayerScreen();
				UpdateScoreBoard();
			}
		}
	}
	UpdateHealthBar(1.0f);
}

void APlayerCharacter::UpdatePlayerScores(bool bUpdateKills)
{
	if (IsLocallyControlled())
	{
		UpdatePlayerScoresImplementation(bUpdateKills);
	}
	else
	{
		ServerUpdatePlayerScores(bUpdateKills);
	}
}

FScores APlayerCharacter::GetPlayerScores()
{
	return PlayerScores;
}

void APlayerCharacter::SetPlayerScores(const FScores& Scores)
{
	PlayerScores = Scores;
	PlayerScores.CurrentKills = 0;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, PlayerScores);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
		DrawUI();
	}
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::FireWeapon);
		Input->BindAction(ReloadAction, ETriggerEvent::Started, this, &ABaseCharacter::Reload);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FVector ForwardVector = GetActorForwardVector();
	AddMovementInput(ForwardVector, MovementVector.X);
	const FVector RightVector = GetActorRightVector();
	AddMovementInput(RightVector, MovementVector.Y);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>() * LookSensitivity;
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::FireWeapon(const FInputActionValue& Value)
{
	FVector CameraPosition;
	FRotator CameraRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CameraPosition, CameraRotation);
	const FVector CameraForward = UKismetMathLibrary::GetForwardVector(CameraRotation);
	if (BulletStartPosition)
	{
		Fire(BulletStartPosition->GetComponentLocation() + 10000.0f * CameraForward);
	}
}

void APlayerCharacter::UpdatePlayerScoresImplementation(bool bUpdateKills)
{
	if (bUpdateKills)
	{
		PlayerScores.CurrentKills += 1;
		if (PlayerScores.CurrentKills > PlayerScores.HighestKills)
		{
			PlayerScores.HighestKills = PlayerScores.CurrentKills;
		}
	}
	else
	{
		PlayerScores.Deaths += 1;
	}
}

void APlayerCharacter::ServerUpdatePlayerScores_Implementation(bool bUpdateKills)
{
	UpdatePlayerScoresImplementation(bUpdateKills);
}


