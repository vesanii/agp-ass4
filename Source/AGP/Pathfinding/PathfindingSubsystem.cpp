// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingSubsystem.h"
#include "EngineUtils.h"

void UPathfindingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	PopulateNodes();
}

TArray<FVector> UPathfindingSubsystem::GetRandomPath(const FVector& StartLocation)
{
	ANavigationNode* NearestNode = FindNearestNode(StartLocation);
	if (NearestNode != nullptr)
	{
		return GetPath(NearestNode, GetRandomNode());
	}
	return TArray<FVector>();
}

TArray<FVector> UPathfindingSubsystem::GetPath(const FVector& StartLocation, const FVector& TargetLocation)
{
	return GetPath(FindNearestNode(StartLocation), FindNearestNode(TargetLocation));
}

TArray<FVector> UPathfindingSubsystem::GetPathAwayFrom(const FVector& StartLocation, const FVector& TargetLocation)
{
	return GetPath(FindNearestNode(StartLocation), FindFurthestNode(TargetLocation));
}

TArray<FVector> UPathfindingSubsystem::GetWaypointPositions()
{
	TArray<FVector> Waypoints;
	for (auto Node : Nodes)
	{
		Waypoints.Push(Node->GetActorLocation());
	}
	return Waypoints;
}

void UPathfindingSubsystem::PopulateNodes()
{
	Nodes.Empty();
	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		Nodes.Push(*It);
		//UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *(*It)->GetActorNameOrLabel(), *(*It)->GetActorLocation().ToString());
	}
}

ANavigationNode* UPathfindingSubsystem::GetRandomNode()
{
	if (!Nodes.IsEmpty())
	{
		return Nodes[FMath::RandRange(0, Nodes.Num()-1)];
	}
	return nullptr;
}

ANavigationNode* UPathfindingSubsystem::FindNearestNode(const FVector& TargetLocation)
{
	if (!Nodes.IsEmpty())
	{
		ANavigationNode* NearestNode{};
		double FarthestDistance = 0.0;
		for (auto Node : Nodes)
		{
			if (FVector::Distance(Node->GetActorLocation(), TargetLocation) > FarthestDistance)
			{
				FarthestDistance = FVector::Distance(Node->GetActorLocation(), TargetLocation);
			}
		}

		double NearestDistance = FarthestDistance;

		for (auto Node : Nodes)
		{
			if (FVector::Distance(Node->GetActorLocation(), TargetLocation) < NearestDistance)
			{
				NearestDistance = FVector::Distance(Node->GetActorLocation(), TargetLocation);
				NearestNode = Node;
			}
		}
		return NearestNode;
	}
	return nullptr;
}

ANavigationNode* UPathfindingSubsystem::FindFurthestNode(const FVector& TargetLocation)
{
	if (!Nodes.IsEmpty())
	{
		ANavigationNode* FurthestNode{};
		double FarthestDistance = 0.0;
		for (auto Node : Nodes)
		{
			if (FVector::Distance(Node->GetActorLocation(), TargetLocation) > FarthestDistance)
			{
				FarthestDistance = FVector::Distance(Node->GetActorLocation(), TargetLocation);
				FurthestNode = Node;
			}
		}
		return FurthestNode;
	}
	return nullptr;
}

TArray<FVector> UPathfindingSubsystem::GetPath(ANavigationNode* StartNode, ANavigationNode* EndNode)
{
	FVector Target = EndNode->GetActorLocation();

	TArray<ANavigationNode*> OpenSet{};
	OpenSet.Push(StartNode);

	for (auto Node : Nodes)
	{
		Node->NodeScores["GScore"] = double(INFINITY);
		Node->NodeScores["HScore"] = FVector::Distance(Node->GetActorLocation(), Target);
		Node->NodeScores["FScore"] = double(INFINITY);
	}
	StartNode->NodeScores["GScore"] = 0.0;
	StartNode->NodeScores["HScore"] = FVector::Distance(StartNode->GetActorLocation(), Target);
	StartNode->NodeScores["FScore"] = StartNode->NodeScores["GScore"] + StartNode->NodeScores["HScore"];
	
	ANavigationNode* CurrentNode = OpenSet[0];

	while (!OpenSet.IsEmpty())
	{
		double LowestFScore = double(INFINITY);
		for (ANavigationNode* Node : OpenSet) {
			if (Node->NodeScores["FScore"] < LowestFScore) 
			{
				CurrentNode = Node;
				LowestFScore = Node->NodeScores["FScore"];
			}
		}

		OpenSet.Remove(CurrentNode);

		if (CurrentNode == EndNode)
		{
			OpenSet.Empty();
			return ReconstructPath(StartNode, EndNode); 
		}
		TArray<ANavigationNode*> ConnectedNodes = CurrentNode->GetConnectedNodes();
		for (ANavigationNode* ConnectedNode: ConnectedNodes)
		{
			double TentativeGScore = CurrentNode->NodeScores["GScore"] + FVector::Distance(CurrentNode->GetActorLocation(), ConnectedNode->GetActorLocation());

			if (TentativeGScore < ConnectedNode->NodeScores["GScore"])
			{
				ConnectedNode->CameFrom = CurrentNode;
				ConnectedNode->NodeScores["GScore"] = TentativeGScore;
				ConnectedNode->NodeScores["HScore"] = FVector::Distance(ConnectedNode->GetActorLocation(), Target);
				ConnectedNode->NodeScores["FScore"] = ConnectedNode->NodeScores["GScore"] + ConnectedNode->NodeScores["HScore"];
				if (!OpenSet.Contains(ConnectedNode))
				{
					OpenSet.Push(ConnectedNode);
				}
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("GetPath Failure"));
	return TArray<FVector>();
}

TArray<FVector> UPathfindingSubsystem::ReconstructPath(ANavigationNode* StartNode, ANavigationNode* EndNode)
{
	TArray<FVector> Path{};
	Path.Push(EndNode->GetActorLocation());
	auto CurrentNode = EndNode;

	while (CurrentNode != StartNode)
	{
		CurrentNode = CurrentNode->CameFrom;
		Path.Push(CurrentNode->GetActorLocation());
	}

	return Path;
}
