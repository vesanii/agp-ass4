// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Senses");
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePawn);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	//if (PathfindingSubsystem)
	//{
	//	CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	//}

	InstantiateStates();
	ChangeState(PatrolState);
}

void AEnemyCharacter::MoveAlongPath()
{
	if (!CurrentPath.IsEmpty())
	{
		FVector MovementDirection = CurrentPath[CurrentPath.Num() - 1] - GetActorLocation();
		MovementDirection.Normalize();
		AddMovementInput(MovementDirection);
		if (FVector::Distance(GetActorLocation(), CurrentPath[CurrentPath.Num() - 1]) < 100.0f)
		{
			CurrentPath.Pop();
		}
	}
}

void AEnemyCharacter::OnSeePawn(APawn* Pawn)
{
	APlayerCharacter* Target = Cast<APlayerCharacter>(Pawn);
	if (Target)
	{
		SensedCharacter = Target;
		//UE_LOG(LogTemp, Warning, TEXT("Sensed Player"));
	}
	AWeaponPickup* TargetWeapon = Cast<AWeaponPickup>(Pawn);
}

void AEnemyCharacter::UpdateSight()
{
	if (!SensedCharacter)
	{
		return;
	}
	LastKnownCharacterLocation = SensedCharacter->GetActorLocation();
	if (!PawnSensingComponent->HasLineOfSightTo(SensedCharacter))
	{
		//UE_LOG(LogTemp, Warning, TEXT("LostPlayer"));
		SensedCharacter = nullptr;
	}
}

float AEnemyCharacter::GetHealth()
{
	return CharacterHealth->GetCurrentHealthPercentage();
}

void AEnemyCharacter::FindWeaponPickup()
{
	for (TActorIterator<AWeaponPickup> It(GetWorld()); It; ++It)
	{
		SensedWeapon = *It;
	}
}

void AEnemyCharacter::FindHealthPickup()
{
	for (TActorIterator<AHealthPickup> It(GetWorld()); It; ++It)
	{
		SensedHealUp = *It;
	}
}

void AEnemyCharacter::InstantiateStates()
{
	PatrolState = NewObject<UPatrolState>(this);
	EngageState = NewObject<UEngageState>(this);
	EvadeState = NewObject<UEvadeState>(this);
	UnarmedState = NewObject<UUnarmedState>(this);
	DeadState = NewObject<UDeadState>(this);
	InvestigateState = NewObject<UInvestigateState>(this);
	InjuredState = NewObject<UInjuredState>(this);
	StunnedState = NewObject<UStunnedState>(this);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasDied())
	{
		UpdateSight();
		if (ActiveState && this)
		{
			ActiveState->Update(this, DeltaTime);
		}
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::CreateRandomPath()
{
	if (PathfindingSubsystem)
	{
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	}
}

void AEnemyCharacter::CreatePathTo(AActor* Target)
{
	if (PathfindingSubsystem)
	{
		CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), Target->GetActorLocation());
	}
}

void AEnemyCharacter::CreatePathTo(FVector Location)
{

	if (PathfindingSubsystem)
	{
		CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), Location);
	}
}

void AEnemyCharacter::CreatePathAwayFrom(APawn* Target)
{
	if (PathfindingSubsystem)
	{
		CurrentPath = PathfindingSubsystem->GetPathAwayFrom(GetActorLocation(), Target->GetActorLocation());
	}
}

void AEnemyCharacter::EmptyCurrentPath()
{
	CurrentPath.Empty();
}

void AEnemyCharacter::Investigate(const float& DeltaTime)
{
	FRotator CurrentActorRotation = GetActorRotation();
	CurrentActorRotation.Yaw += DeltaTime * 120.0f;
	SetActorRotation(CurrentActorRotation);
}

void AEnemyCharacter::ChangeState(UBaseState* NewState)
{
	if (ActiveState)
	{
		ActiveState->Exit(this);
	}
	ActiveState = NewState;
	if (ActiveState)
	{
		ActiveState->Entry(this);
	}
}