// Fill out your copyright notice in the Description page of Project Settings.


#include "BallGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"

#include "Kismet/GameplayStatics.h"



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

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Balls"), BallGen);
	BallGen.Add(nullptr);
	Balls = BallGen[0];
	MyLocation = Generator->GetRelativeLocation();
	MyRotation = Generator->GetRelativeRotation();
	Balls->SetActorLocation(MyLocation);
	Balls->SetActorHiddenInGame(true);
}

// Called every frame
void ABallGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fps += 1.0f;

	AExplodedBalls* NewBall = nullptr;
	if (int(fps) % period == 0 && Balls != nullptr)
	{
		NewBall = generate(Balls, FVector(200 * FMath::Sin(-MyRotation.Yaw / 180 * PI), 200 * FMath::Cos(-MyRotation.Yaw / 180 * PI), 0));
		NewBall->Ball->AddImpulse(FVector(impulse * FMath::Sin(-MyRotation.Yaw / 180 * PI), impulse * FMath::Cos(-MyRotation.Yaw / 180 * PI), 0));
	}
}

AExplodedBalls* ABallGenerator::generate(AActor* Actortype, FVector Location, FRotator Rotation)
{
	FActorSpawnParameters Parameters;

	AExplodedBalls* OtherBall = nullptr;


	Parameters.Template = Actortype;

	Parameters.Owner = GetOwner();

	UWorld* World = GetWorld();

	if (IsValid(World))
	{

		OtherBall = World->SpawnActor<class AExplodedBalls>(Actortype->GetClass(), Location, Rotation, Parameters);

		UE_LOG(LogTemp, Log, TEXT("%s had been created!"), *OtherBall->GetName());

		FVector BallLocation = OtherBall->Ball->GetRelativeLocation();
		FRotator BallRotation = OtherBall->Ball->GetRelativeRotation();

		UE_LOG(LogTemp, Log, TEXT("Instance %s is now locating at: (%f,%f,%f)"), *OtherBall->GetName(), BallLocation.X, BallLocation.Y, BallLocation.Z);
		UE_LOG(LogTemp, Log, TEXT("Instance %s has rotation: (%f,%f,%f)"), *OtherBall->GetName(), BallRotation.Roll, BallRotation.Pitch, BallRotation.Yaw);
		UE_LOG(LogTemp, Log, TEXT("Name:%s"), *OtherBall->GetName());

		OtherBall->SetActorHiddenInGame(false);
		OtherBall->SetLifeSpan(span);
		OtherBall->Ball->SetSimulatePhysics(true);
		OtherBall->Ball->BodyInstance.bOverrideMass = true;
		OtherBall->Ball->BodyInstance.SetMassOverride(2.0f);
	}


	return OtherBall;
}


void ABallGenerator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Foward", this, &ABallGenerator::Move_Foward);
	InputComponent->BindAxis("Backward", this, &ABallGenerator::Move_Foward);
	InputComponent->BindAxis("Left", this, &ABallGenerator::Move_Right);
	InputComponent->BindAxis("Right", this, &ABallGenerator::Move_Right);
	InputComponent->BindAxis("Left_Turn", this, &ABallGenerator::Turn_Right);
	InputComponent->BindAxis("Right_Turn", this, &ABallGenerator::Turn_Right);

	InputComponent->BindAction("Exit", IE_Pressed, this, &ABallGenerator::Escape);

}

void ABallGenerator::Move_Foward(float AxisValue)
{
	//Make My pawn moves along the X axis
	Generator->AddRelativeLocation(FVector(AxisValue * move, 0, 0));
	MyLocation = Generator->GetRelativeLocation();
	Balls->SetActorLocation(MyLocation);
}

void ABallGenerator::Move_Right(float AxisValue)
{
	//Make My pawn moves along the Y axis
	Generator->AddRelativeLocation(FVector(0, AxisValue * move, 0));
	MyLocation = Generator->GetRelativeLocation();
	Balls->SetActorLocation(MyLocation);
}

void ABallGenerator::Turn_Right(float AxisValue)
{
	//Make My pawn turns
	AddControllerYawInput(AxisValue * rotate);
	MyRotation = Generator->GetRelativeRotation();
}

void ABallGenerator::Escape()
{
	//Post Endgame Request
	FGenericPlatformMisc::RequestExit(false);
	UE_LOG(LogTemp, Log, TEXT("Exit!"));
}

