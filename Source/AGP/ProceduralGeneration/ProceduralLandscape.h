// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetProceduralMeshLibrary.h"
#include "AGP/Pathfinding/NavigationNode.h"
#include "ProceduralLandscape.generated.h"

UCLASS()
class AGP_API AProceduralLandscape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralLandscape();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;
	UPROPERTY(EditAnywhere)
	bool bShouldRegenerate;
	UPROPERTY(EditAnywhere)
	bool bShouldHaveDiagonalNodeConnections{true};
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector2D> UVCoords;
	UPROPERTY()
	TArray<FVector> Normals;
	UPROPERTY()
	TArray<FProcMeshTangent> Tangents;
	UPROPERTY(EditAnyWhere)
	int32 Width = 10;
	UPROPERTY(EditAnywhere)
	int32 Height = 10;
	UPROPERTY(EditAnywhere)
	float VertexSpacing = 1000.0f;
	UPROPERTY(EditAnywhere)
	float PerlinScale = 1000.0f;
	UPROPERTY(EditAnywhere)
	float PerlinRoughness = 0.00012f;
	UPROPERTY(VisibleAnywhere)
	float PerlinOffset;
	UPROPERTY()
	TArray<ANavigationNode*> Nodes;

	void CreateSimplePlane();

private: 
	void PopulateNodes();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ClearLandscape();
	virtual bool ShouldTickIfViewportsOnly() const override;
	void GenerateLandscape();
};
