// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) 
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
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
	}
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	}
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	}
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::FireWeapon);
	}
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ReloadWeapon);
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>(); // creates a variable to represent the character's movement by getting the fvector each time a movement key is pressed
	const FVector ForwardVector = GetActorForwardVector(); // ^^ for forward momentum
	AddMovementInput(ForwardVector, MovementVector.X);
	const FVector RightVector = GetActorRightVector(); // ^^ for side momentum
	AddMovementInput(RightVector, MovementVector.Y);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookVector.X * Sensitivity);
	AddControllerPitchInput(LookVector.Y * Sensitivity);
}

void APlayerCharacter::FireWeapon(const FInputActionValue& Value)
{
	FVector CameraPosition; //where the camera is
	FRotator CameraRotation; //which way the camera is facing
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CameraPosition, CameraRotation); //sets these values
	const FVector CameraForward = UKismetMathLibrary::GetForwardVector(CameraRotation); //forward vector of the camera, which way will it go
	if (BulletStartPosition)
	{
		Fire(BulletStartPosition->GetComponentLocation() + 10000.0f * CameraForward); //FireAtLocation
	}
}

void APlayerCharacter::ReloadWeapon(const FInputActionValue& Value)
{
	if (WeaponComponent)
	{
		WeaponComponent->ReloadMagazine();
	}
}
