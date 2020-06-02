/*
@Project: Physics
@Owner: Donspaces
Date: Jun 2th, 2020 11:59 PM
*/


#include "ExplodedBalls.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AExplodedBalls::AExplodedBalls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Ball->SetupAttachment(RootComponent);

	UMaterial* BallMaterial;
	BallMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/StarterContent/Materials/M_Wood_Pine")).Get();

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (BallMesh.Succeeded())
	{
		Ball->SetStaticMesh(BallMesh.Object);
		Ball->SetMaterial(0, BallMaterial);
		Tags.Add(FName("Balls"));
	}
}

// Called when the game starts or when spawned
void AExplodedBalls::BeginPlay()
{
	Super::BeginPlay();
	fps = 0.0f;
}

// Called every frame
void AExplodedBalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fps += 1.0f;
}
