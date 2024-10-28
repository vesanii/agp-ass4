// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupBounceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UPickupBounceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupBounceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void TickFloatUpObject(const float& DeltaTime);
	void TickFloatDownObject(const float& DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere)
	float BounceSpeed;

private:
	bool bFloatingUp = true;
	float timer = 1.0f;
};
