// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationNode.h"

// Sets default values
ANavigationNode::ANavigationNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Node"));
	SetRootComponent(LocationComponent);
}

bool ANavigationNode::ShouldTickIfViewportsOnly() const
{
	return true;
}

TArray<ANavigationNode*> ANavigationNode::GetConnectedNodes()
{
	return ConnectedNodes;
}

void ANavigationNode::PushConnectedNode(ANavigationNode* Node)
{
	if (Node)
	{
		ConnectedNodes.Push(Node);
	}
	if (!Node)
	{
		UE_LOG(LogTemp, Warning, TEXT("empty node"));
	}
}

// Called when the game starts or when spawned
void ANavigationNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavigationNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 50.0f, 4, FColor::Blue, false);

	for (auto* node: ConnectedNodes)
	{
		if (node != nullptr)
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), node->GetActorLocation(), FColor::Blue); 
		}
	}
}

