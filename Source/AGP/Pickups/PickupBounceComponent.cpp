// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBounceComponent.h"

// Sets default values for this component's properties
UPickupBounceComponent::UPickupBounceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPickupBounceComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

void UPickupBounceComponent::TickFloatUpObject(const float& DeltaTime)
{
	FVector CurrentActorLocation = GetOwner()->GetActorLocation();
	CurrentActorLocation.Z += DeltaTime * BounceSpeed;
	GetOwner()->SetActorLocation(CurrentActorLocation);
}

void UPickupBounceComponent::TickFloatDownObject(const float& DeltaTime)
{
	FVector CurrentActorLocation = GetOwner()->GetActorLocation();
	CurrentActorLocation.Z -= DeltaTime * BounceSpeed;
	GetOwner()->SetActorLocation(CurrentActorLocation);
}


// Called every frame
void UPickupBounceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bFloatingUp)
	{
		TickFloatUpObject(DeltaTime);
		timer -= DeltaTime;
		if (timer <= 0)
		{
			bFloatingUp = false;
			timer = 1.0f;
		}
	}
	if (!bFloatingUp)
	{
		TickFloatDownObject(DeltaTime);
		timer -= DeltaTime;
		if (timer <= 0)
		{
			bFloatingUp = true;
			timer = 1.0f;
		}
	}
	
	// ...
}

