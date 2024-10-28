// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralLandscape.h"
#include "EngineUtils.h"

// Sets default values
AProceduralLandscape::AProceduralLandscape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	SetRootComponent(ProceduralMesh);
}

// Called when the game starts or when spawned
void AProceduralLandscape::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralLandscape::CreateSimplePlane()
{
	Vertices.Push(FVector(0.0f, 0.0f, FMath::RandRange(-500.0f, 500.0f)));
	Vertices.Push(FVector(1000.0f, 0.0f, FMath::RandRange(-500.0f, 500.0f)));
	Vertices.Push(FVector(0.0f, 1000.0f, FMath::RandRange(-500.0f, 500.0f)));
	Vertices.Push(FVector(1000.0f, 1000.0f, FMath::RandRange(-500.0f, 500.0f)));

	for (FVector &Vertex : Vertices)
	{
		DrawDebugSphere(GetWorld(), Vertex, 50.0f, 16, FColor::Blue, true);
	}
	Triangles.Push(0);
	Triangles.Push(2);
	Triangles.Push(1);
	Triangles.Push(1);
	Triangles.Push(2);
	Triangles.Push(3);

	UVCoords.Push(FVector2d(0.0f, 0.0f));
	UVCoords.Push(FVector2d(1.0f, 0.0f));
	UVCoords.Push(FVector2d(0.0f, 1.0f));
	UVCoords.Push(FVector2d(1.0f, 1.0f));

	if (ProceduralMesh)
	{
		ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVCoords,
			TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	}
}

void AProceduralLandscape::PopulateNodes()
{
	Nodes.Empty();
	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		Nodes.Push(*It);
	}
}

// Called every frame
void AProceduralLandscape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShouldRegenerate)
	{
		ClearLandscape();
		GenerateLandscape();
		bShouldRegenerate = false;
	}
}

void AProceduralLandscape::ClearLandscape()
{
	Vertices.Empty();
	Triangles.Empty();
	UVCoords.Empty();
	if (!Nodes.IsEmpty())
	{
		for (ANavigationNode* Node : Nodes)
		{
			if (Node)
			{
				Node->Destroy();
			}
		}
	}

	ProceduralMesh->ClearMeshSection(0);
	UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());
}

bool AProceduralLandscape::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AProceduralLandscape::GenerateLandscape()
{
	PerlinOffset = FMath::FRandRange(-1'000'000.0f, 1'000'000.0f);
	for (float y = 0; y < Height; y++)
	{
		for (float x = 0; x < Width; x++)
		{
			float z = FMath::PerlinNoise2D(FVector2d((x * VertexSpacing) * PerlinRoughness + PerlinOffset, (y * VertexSpacing) * PerlinRoughness) + PerlinOffset);
			FVector Vertex{ x * VertexSpacing, y * VertexSpacing, z*PerlinScale};
			Vertices.Push(Vertex);
			UVCoords.Push(FVector2D(x, y));
			ANavigationNode* Node = GetWorld()->SpawnActor<ANavigationNode>();
			Node->SetActorLocation(Vertex);
			//DrawDebugSphere(GetWorld(), Vertex, 50.0f, 16, FColor::Blue, true);
		}
	}
	PopulateNodes();

	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			int Current = y * Width + x;
			int Above = (y + 1) * Width + x;
			int Left = y * Width + (x + 1);
			int AboveAndLeft = (y + 1) * Width + (x + 1);
			int AboveAndRight = (y + 1) * Width + (x - 1);
			int StartOfRow = Width * y;
			int EndOfRow = (Width - 1) + (y * Width);

			if (Above < Nodes.Num())
			{
				Nodes[Current]->PushConnectedNode(Nodes[Above]);
				Nodes[Above]->PushConnectedNode(Nodes[Current]);

				if (Current != EndOfRow)
				{
					Nodes[Current]->PushConnectedNode(Nodes[Left]);
					Nodes[Left]->PushConnectedNode(Nodes[Current]);
					if (bShouldHaveDiagonalNodeConnections)
					{
						Nodes[Current]->PushConnectedNode(Nodes[AboveAndLeft]);
						Nodes[AboveAndLeft]->PushConnectedNode(Nodes[Current]);
					}

					Triangles.Push(Current);
					Triangles.Push(Above);
					Triangles.Push(Left);

					Triangles.Push(Left);
					Triangles.Push(Above);
					Triangles.Push(AboveAndLeft);
				}


				if (Current != StartOfRow && bShouldHaveDiagonalNodeConnections) 
				{
					Nodes[Current]->PushConnectedNode(Nodes[AboveAndRight]);
					Nodes[AboveAndRight]->PushConnectedNode(Nodes[Current]);
				}
			}
		}
	}
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVCoords, Normals, Tangents);

	if (ProceduralMesh)
	{
		ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVCoords,
			TArray<FColor>(), Tangents, true);
	}
}

