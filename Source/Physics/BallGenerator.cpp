/*
@Project: Physics
@Owner: Donspaces
Date: Jun 4th, 2020 2:12 PM
*/


#include "BallGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Engine.h"

// Sets default values
ABallGenerator::ABallGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Box->SetBoxExtent(FVector(72.0f, 128.0f, 72.0f));
	Box->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = Box;

	Generator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Generator"));
	Generator->SetupAttachment(RootComponent);
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bUseControllerRotationYaw = true;

	Exploded = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Exploded"));
	Exploded->SetupAttachment(Generator);
	Exploded->bAutoActivate = false;
	Exploded->SetRelativeLocation(-GetActorRightVector() * 240.0f);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplodedParticles(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (ExplodedParticles.Succeeded())
	{
		Exploded->SetTemplate(ExplodedParticles.Object);

	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Meshes/Canon.Canon"));
	if (CubeMesh.Succeeded())
	{
		Generator->SetStaticMesh(CubeMesh.Object);
		Generator->SetRelativeScale3D(FVector(0.5f));
		Generator->RelativeLocation.Z = -71.0f;
		Generator->SetWorldRotation(FRotator(0, 180.0f, 0));
	}

	MovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void ABallGenerator::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Balls"), BallGen);
	BallGen.Add(nullptr);
	Balls = BallGen[0];
	MyLocation = GetActorLocation();
	MyRotation = GetActorRotation();
	Balls->SetActorHiddenInGame(true);

	GEngine->AddOnScreenDebugMessage(0, MAX_FLT, FColor::Green, FString::Printf(TEXT("Impulse:%f"), impulse));
	GEngine->AddOnScreenDebugMessage(1, MAX_FLT, FColor::Green, FString::Printf(TEXT("Period:%d"), period));
}

// Called every frame
void ABallGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fps += 1.0f;
	AExplodedBalls* NewBall = nullptr;
	if (int(fps) % period == 0 && Balls != nullptr)
	{
		if (Exploded && Exploded->Template)
		{
			Exploded->ToggleActive();
		}
		NewBall = generate(Balls, MyLocation * FVector(1, 1, 0) + GetActorRightVector() * 200.0f);
		NewBall->Ball->AddImpulse(GetActorRightVector() * impulse * 200.0f);
	}
}

AExplodedBalls* ABallGenerator::generate(AActor* Actortype, FVector Location, FRotator Rotation)
{

	AExplodedBalls* OtherBall = nullptr;

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		OtherBall = World->SpawnActor<class AExplodedBalls>(Actortype->GetClass(), Location, Rotation);

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

UPawnMovementComponent* ABallGenerator::GetMovementComponent() const
{
	
	return MovementComponent;
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
	InputComponent->BindAxis("Key_Up", this, &ABallGenerator::Increase_Impulse);
	InputComponent->BindAxis("Key_Down", this, &ABallGenerator::Decrease_Impulse);

	InputComponent->BindAction("Key_Left", IE_Pressed, this, &ABallGenerator::Decrease_Period);
	InputComponent->BindAction("Key_Right", IE_Pressed, this, &ABallGenerator::Increase_Period);

	InputComponent->BindAction("Exit", IE_Pressed, this, &ABallGenerator::Escape);

}

void ABallGenerator::Move_Foward(float AxisValue)
{
	//Make My pawn moves along the X axis
	if (MovementComponent && MovementComponent->UpdatedComponent == RootComponent)
	{
		MovementComponent->AddInputVector(FVector::ForwardVector * AxisValue * move);
	}

	MyLocation = GetActorLocation();
}

void ABallGenerator::Move_Right(float AxisValue)
{
	//Make My pawn moves along the Y axis
	if (MovementComponent && MovementComponent->UpdatedComponent == RootComponent)
	{
		MovementComponent->AddInputVector(FVector::RightVector * AxisValue * move);
	}

	MyLocation = GetActorLocation();
}

void ABallGenerator::Turn_Right(float AxisValue)
{
	//Make My pawn turns
	AddControllerYawInput(AxisValue * rotate);
	MyRotation = GetActorRotation();
}

void ABallGenerator::Increase_Impulse(float AxisValue)
{
	if (impulse < 60000)
	{
		impulse += AxisValue * updown_speed;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, FString::Printf(TEXT("Impulse:%f"), impulse));
	}
}

void ABallGenerator::Decrease_Impulse(float AxisValue)
{
	if (impulse > 0)
	{
		impulse += AxisValue * updown_speed;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Green, FString::Printf(TEXT("Impulse:%f"), impulse));
	}
}

void ABallGenerator::Decrease_Period()
{
	if (period > 20)
	{
		period -= 10;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, MAX_FLT, FColor::Green, FString::Printf(TEXT("Period:%d"), period));
	}
}

void ABallGenerator::Increase_Period()
{
	if (period < 500)
	{
		period += 10;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, MAX_FLT, FColor::Green, FString::Printf(TEXT("Period:%d"), period));
	}
}

void ABallGenerator::Escape()
{
	//Post Endgame Request
	FGenericPlatformMisc::RequestExit(false);
	UE_LOG(LogTemp, Log, TEXT("Exit!"));
}

