// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EngineUtils.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"
#include "AGP/Pathfinding/NavigationNode.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	if (PathfindingSubsystem)
	{
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the PathfindingSubsystem"))
	}
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSensedPawn);
	}
	if (HealthComponent)
	{
		HealthComponent->OnTakeDamage.AddDynamic(this, &AEnemyCharacter::OnDamageTaken);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HealthComponent is null in BeginPlay!"));
	}
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

void AEnemyCharacter::TickPatrol()
{
	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	}
	MoveAlongPath();
}

void AEnemyCharacter::TickEngage()
{
	
	if (!SensedCharacter) return;
	
	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), SensedCharacter->GetActorLocation());
	}
	MoveAlongPath();
	Fire(SensedCharacter->GetActorLocation());
}

void AEnemyCharacter::TickEvade()
{
	if (!SensedCharacter) return;

	if (CurrentPath.IsEmpty())
	{
		CurrentPath = PathfindingSubsystem->GetPathAway(GetActorLocation(), SensedCharacter->GetActorLocation());
	}
	MoveAlongPath();
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
	if (!SensedCharacter) return;
	if (PawnSensingComponent)
	{
		if (!PawnSensingComponent->HasLineOfSightTo(SensedCharacter))
		{
			SensedCharacter = nullptr;
			UE_LOG(LogTemp, Display, TEXT("Lost Player"))
		}
	}
}


void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if HealthComponent is valid before using it
	if (!HealthComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("HealthComponent is null!"));
		return;
	}

	UpdateSight();

	FString DebugString = FString::Printf(TEXT("Health: %.1f%%\nState: %s"),
		HealthComponent->GetCurrentHealthPercentage() * 100.0f,
		*GetEnemyStateName(CurrentState));

    DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 100), DebugString, nullptr, FColor::White, 0.0f, true);

	if (HealthComponent->IsDead())
	{
		// Prevent any further movement or actions
		return;
	}

	HealthComponent->UpdateHealing(DeltaTime);

	


    switch (CurrentState)
    {
    case EEnemyState::Patrol:
        TickPatrol();
        if (SensedCharacter)
        {
            if (HealthComponent->GetCurrentHealthPercentage() >= 0.4f)
            {
                CurrentState = EEnemyState::Engage;
            }
            else
            {
                CurrentState = EEnemyState::Evade;
            }
            CurrentPath.Empty();
        }
        break;
    case EEnemyState::Engage:
        TickEngage();
        if (HealthComponent->GetCurrentHealthPercentage() < 0.4f)
        {
            CurrentPath.Empty();
            CurrentState = EEnemyState::Evade;
        }
        else if (!SensedCharacter)
        {
            CurrentState = EEnemyState::Patrol;
        }
        break;
    case EEnemyState::Evade:
        TickEvade();
        if (!SensedCharacter)
        {
            CurrentPath.Empty();
            CurrentState = EEnemyState::TakeCover;
        }
        break;
    case EEnemyState::TakeCover:
        TickTakeCover();
        if (HealthComponent->GetCurrentHealthPercentage() >= 0.65f)
        {
            CurrentState = SensedCharacter ? EEnemyState::Engage : EEnemyState::Patrol;
            CurrentPath.Empty();

            bIsInCover = false;
        }
        break;
    }
}





// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APlayerCharacter* AEnemyCharacter::FindPlayer() const
{
	APlayerCharacter* Player = nullptr;
	for (TActorIterator<APlayerCharacter> It(GetWorld()); It; ++It)
	{
		Player = *It;
		break;
	}
	if (!Player)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find the Player Character in the world."))
	}
	return Player;
}

void AEnemyCharacter::TickTakeCover()
{
    if (CurrentPath.IsEmpty() && PathfindingSubsystem)
    {
        // Find the nearest cover node relative to the AI's current location
        ANavigationNode* CoverNode = PathfindingSubsystem->FindNearestCoverNode(GetActorLocation());
        
        if (CoverNode)
        {
            CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(), CoverNode->GetActorLocation());
        }
    }

    if (!CurrentPath.IsEmpty())
    {
        MoveAlongPath();
    }

    if (PathfindingSubsystem)
    {
        ANavigationNode* NearestCoverNode = PathfindingSubsystem->FindNearestCoverNode(GetActorLocation());
        if (NearestCoverNode)
        {
            float DistanceToCover = FVector::Distance(GetActorLocation(), NearestCoverNode->GetActorLocation());

            if (DistanceToCover <= 100.0f && !bIsInCover)
            {
                bIsInCover = true;

                // Start healing immediately when the AI enters cover   
                if (HealthComponent)
                {
                    HealthComponent->StartImmediateHealing();
                }
            }
        }
    }
}


bool AEnemyCharacter::IsEnemyInCover() const
{
    return bIsInCover;
}

void AEnemyCharacter::OnDamageTaken()
{
	if (CurrentState == EEnemyState::TakeCover)
	{
		CurrentState = EEnemyState::Evade;
		CurrentPath.Empty();
		bIsInCover = false;
	}
}

FString AEnemyCharacter::GetEnemyStateName(EEnemyState State) const
{
	switch (State)
	{
	case EEnemyState::Patrol:
		return "Patrol";
	case EEnemyState::Engage:
		return "Engage";
	case EEnemyState::Evade:
		return "Evade";
	case EEnemyState::TakeCover:
		return "Taking Cover";
	default:
		return "Unknown";
	}
}
