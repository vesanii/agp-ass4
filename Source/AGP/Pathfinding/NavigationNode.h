// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationNode.generated.h"

UCLASS()
class AGP_API ANavigationNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANavigationNode();
	virtual bool ShouldTickIfViewportsOnly() const override;
	TArray<ANavigationNode*> GetConnectedNodes();
	void PushConnectedNode(ANavigationNode* Node);

	TMap<FString, double> NodeScores{ {"GScore", 0.0}, {"HScore", 0.0}, {"FScore", 0.0} };
	ANavigationNode* CameFrom{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<ANavigationNode*> ConnectedNodes;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* LocationComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
