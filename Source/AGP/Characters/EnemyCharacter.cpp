// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include <Net/UnrealNetwork.h>

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// DO NOTHING IF NOT ON THE SERVER
	if (GetLocalRole() != ROLE_Authority) return;
	SensedCharacter.Reset();

	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	if (!PathfindingSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the PathfindingSubsystem"))
	}
	PawnSensingComponent = NewObject<UPawnSensingComponent>(this);
	PawnSensingComponent->RegisterComponent();
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSensedPawn);
		UE_LOG(LogTemp, Warning, TEXT("PawnSensingComponent exists"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find PawnSensingComponent"));
	}
	InstantiateStates();
	ChangeState(PatrolState);
}

void AEnemyCharacter::MoveAlongPath()
{
	// Execute the path. Should be called each tick.

	// If the path is empty do nothing.
	if (CurrentPath.IsEmpty()) return;
	
	// 1. Move towards the current stage of the path.
	//		a. Calculate the direction from the current position to the target of the current stage of the path.
	FVector MovementDirection = CurrentPath[CurrentPath.Num()-1] - GetActorLocation();
	MovementDirection.Normalize();
	//		b. Apply movement in that direction.
	AddMovementInput(MovementDirection);
	// 2. Check if it is close to the current stage of the path then pop it off.
	if (FVector::Distance(GetActorLocation(), CurrentPath[CurrentPath.Num() - 1]) < PathfindingError)
	{
		CurrentPath.Pop();
	}
}

void AEnemyCharacter::OnSensedPawn(APawn* SensedActor)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(SensedActor))
	{
		SensedCharacter = Player;
		UE_LOG(LogTemp, Display, TEXT("Sensed Player"))
	}
}

void AEnemyCharacter::UpdateSight()
{
	if (!SensedCharacter.IsValid()) 
	{
		return;
	}
	LastKnownCharacterLocation = SensedCharacter.Get()->GetActorLocation();
	if (PawnSensingComponent)
	{
		if (!PawnSensingComponent->HasLineOfSightTo(SensedCharacter.Get()))
		{
			SensedCharacter.Reset();
			UE_LOG(LogTemp, Display, TEXT("Lost Player"))
		}
	}
}

float AEnemyCharacter::GetHealth()
{
	return HealthComponent->GetCurrentHealthPercentage();
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
		CurrentPath = PathfindingSubsystem->GetPathAway(GetActorLocation(), Target->GetActorLocation());
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


// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// DO NOTHING UNLESS IT IS ON THE SERVER
    if (GetLocalRole() != ROLE_Authority) return;

    FString DebugString = FString::Printf(TEXT("Health: %.1f%%\nState: %s"),
        HealthComponent ? HealthComponent->GetCurrentHealthPercentage() * 100.0f : 0.0f,
        *GetStateName());

    DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 100), DebugString, nullptr, FColor::White, 0.0f, true);

	bIsInCover = (ActiveState == HideState);
	
    UpdateSight();
    if (ActiveState && this)
    {
        ActiveState->Update(this, DeltaTime);
    }
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::FindWeaponPickup()
{
	AWeaponPickup* ClosestPickup = nullptr;
	float MinDistance = UE_MAX_FLT;
	for (TActorIterator<AWeaponPickup> Pickup(GetWorld()); Pickup; ++Pickup)
	{
		const float Distance = FVector::Distance(GetActorLocation(), Pickup->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPickup = *Pickup;
		}
	}
	SensedWeapon = ClosestPickup;
}

void AEnemyCharacter::FindHealthPickup()
{
	AHealthPickup* ClosestPickup = nullptr;
	float MinDistance = UE_MAX_FLT;
	for (TActorIterator<AHealthPickup> Pickup(GetWorld()); Pickup; ++Pickup)
	{
		const float Distance = FVector::Distance(GetActorLocation(), Pickup->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			ClosestPickup = *Pickup;
		}
	}
	SensedHealUp = ClosestPickup;
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
	HideState = NewObject<UHideState>(this);
}

FString AEnemyCharacter::GetStateName() const
{
    if (ActiveState == PatrolState) return "Patrol";
    if (ActiveState == EngageState) return "Engage";
    if (ActiveState == EvadeState) return "Evade";
    if (ActiveState == UnarmedState) return "Unarmed";
    if (ActiveState == DeadState) return "Dead";
    if (ActiveState == InvestigateState) return "Investigate";
    if (ActiveState == InjuredState) return "Injured";
    if (ActiveState == StunnedState) return "Stunned";
	if (ActiveState == HideState) return "Hiding";
    return "Unknown";
}

bool AEnemyCharacter::IsEnemyInCover() const
{
    if (ActiveState == HideState) {
        return bIsInCover;
    }
    return false; 
}