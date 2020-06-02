// Fill out your copyright notice in the Description page of Project Settings.


#include "BallGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABallGenerator::ABallGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Generator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Generator"));
	Generator->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (CubeMesh.Succeeded())
	{
		Generator->SetStaticMesh(CubeMesh.Object);
	}
}

// Called when the game starts or when spawned
void ABallGenerator::BeginPlay()
{
	Super::BeginPlay();
	Generator->SetHiddenInGame(true);
}

// Called every frame
void ABallGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fps += 1.0f;
	if (int(fps) % 260 == 0)
	{
		generate(FVector(150, 150, 0));
	}
}

AExplodedBalls* ABallGenerator::generate(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters Parameters;

	AExplodedBalls* OtherBall = nullptr;

	Parameters.Template =  OtherBall;

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		OtherBall = World->SpawnActor<class AExplodedBalls>(AExplodedBalls::StaticClass(), Location, Rotation, Parameters);

		UE_LOG(LogTemp, Log, TEXT("%s had been created!"), *OtherBall->GetName());

		OtherBall->SetOwner(GetOwner());
	}


	return OtherBall;
}
