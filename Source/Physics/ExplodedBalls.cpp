// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplodedBalls.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"
#include <ctime>
#include <cstdlib>

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
	}
}

// Called when the game starts or when spawned
void AExplodedBalls::BeginPlay()
{
	Super::BeginPlay();
	fps = 0.0f;

	FVector Location = Ball->GetRelativeLocation();
	FRotator Rotation = Ball->GetRelativeRotation();

	UE_LOG(LogTemp, Log, TEXT("Instance %s is now locating at: (%f,%f,%f)"), *Ball->GetName(), Location.X, Location.Y, Location.Z);
	UE_LOG(LogTemp, Log, TEXT("Instance %s has rotation: (%f,%f,%f)"), *Ball->GetName(), Rotation.Roll, Rotation.Pitch, Rotation.Yaw);
	UE_LOG(LogTemp, Log, TEXT("Parent:%s"), *GetName());

	if (GetName() == "ExplodedBalls_1")
	{
		AExplodedBalls* ExplodeBall[4];
		ExplodeBall[0] = clone(FVector(100, 0, 0));
		ExplodeBall[1] = clone(FVector(0, 100, 0));
		ExplodeBall[2] = clone(FVector(100, 100, 0));
		
		Ball->SetHiddenInGame(true);
		FVector NewLocation = FVector(4870, 3350, 0);
		UE_LOG(LogTemp, Log, TEXT("(%f,%f,%f)"), NewLocation.X, NewLocation.Y, NewLocation.Z);
		for (int i = 0; i <= 2; i++)
		{
			ExplodeBall[i]->Ball->AddRadialImpulse(NewLocation, 200, explode_strength, ERadialImpulseFalloff::RIF_Linear, true);
		}
		srand(time(NULL));
	}
	
}

// Called every frame
void AExplodedBalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fps += 1.0f;
	AExplodedBalls* NewBall = nullptr;
	if (int(fps) % 260 == 0 && GetName() == "ExplodedBalls_1")
	{
		float X_rand, Y_rand, Angle;
		X_rand = rand() % 2001 - 1000;
		Y_rand = rand() % 2001 - 1000;
		Angle = rand() % 360;
		NewBall = clone(FVector(X_rand, Y_rand, 0));
		NewBall->Ball->AddImpulse(FVector(impulse * sin(Angle * PI / 180), impulse * cos(Angle * PI / 180), 0));
	}
}

AExplodedBalls* AExplodedBalls::clone(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters Parameters;

	AExplodedBalls* OtherBall = nullptr;

	Parameters.Template = this;

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		OtherBall = World->SpawnActor<class AExplodedBalls>(GetClass(), Location, Rotation, Parameters);

		UE_LOG(LogTemp, Log, TEXT("%s had been created!"), *OtherBall->GetName());

		OtherBall->SetOwner(GetOwner());
		OtherBall->Ball->SetHiddenInGame(false);
		OtherBall->Ball->SetSimulatePhysics(true);
		OtherBall->Ball->BodyInstance.bOverrideMass = true;
		OtherBall->Ball->BodyInstance.SetMassOverride(2.0f);
	}


	return OtherBall;
}
