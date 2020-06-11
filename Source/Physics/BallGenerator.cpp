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
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Engine/DataTable.h"
#include "PhysicsGameModeBase.h"
#include "Engine/Engine.h"

// Sets default values
ABallGenerator::ABallGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Add Box Collision RootComponent
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	Box->SetBoxExtent(FVector(72.0f, 128.0f, 72.0f));
	Box->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = Box;

	//Add Generator Mesh
	Generator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Generator"));
	Generator->SetupAttachment(RootComponent);
	AutoPossessPlayer = EAutoReceiveInput::Player0;//Setup PlayerControl
	bUseControllerRotationYaw = true;//Enable RotationYaw

	//Add Particle System
	Exploded = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Exploded"));
	Exploded->SetupAttachment(Generator);
	Exploded->bAutoActivate = false;
	Exploded->SetRelativeLocation(-GetActorRightVector() * 240.0f);

	//Add Sound Cue
	Exploded_Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Exploded Sound"));
	Exploded_Sound->SetupAttachment(Generator);
	Exploded_Sound->bAutoActivate = false;
	Exploded_Sound->SetRelativeLocation(-GetActorRightVector() * 240.0f);

	//Create Sound Cue Element
	static ConstructorHelpers::FObjectFinder<USoundCue> ExplodedSoundCue(TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	if (Exploded_Sound->IsValidLowLevelFast() && ExplodedSoundCue.Succeeded())
	{
		Exploded_Sound->SetSound(ExplodedSoundCue.Object);
	}

	//Create Particle System Element
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplodedParticles(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (ExplodedParticles.Succeeded())
	{
		Exploded->SetTemplate(ExplodedParticles.Object);

	}
	
	//Create Static Mesh Element
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Meshes/Canon.Canon"));
	if (CubeMesh.Succeeded())
	{
		Generator->SetStaticMesh(CubeMesh.Object);
		Generator->SetRelativeScale3D(FVector(0.5f));
		Generator->RelativeLocation.Z = -71.0f;
		Generator->SetWorldRotation(FRotator(0, 180.0f, 0));
	}

	//Attach Datatable to Pawn
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/StarterContent/Csv_archive/UserConfig.UserConfig"));
	if (DataTable.Succeeded())
	{
		UserData = DataTable.Object;
		UE_LOG(LogTemp, Log, TEXT("Finding data success!"));
	}
	
	//Modify Raw Input Config
	static const FString ContextString = TEXT("CSV");
	FUserConfig* UserInfo = UserData->FindRow<FUserConfig>(TEXT("User0"), ContextString, true);
	if (UserInfo)
	{
		move = UserInfo->move_config;
		rotate = UserInfo->rotate_config;
		updown_speed = UserInfo->updown_config;
	}

	//Add MovementComponent
	MovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void ABallGenerator::BeginPlay()
{
	Super::BeginPlay();

	//Ball Generation
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Balls"), BallGen);
	BallGen.Add(nullptr);
	Balls = BallGen[0];
	MyLocation = GetActorLocation();
	MyRotation = GetActorRotation();
	Balls->SetActorHiddenInGame(true);

	//Add Impulse and Period Info Disp
	GEngine->AddOnScreenDebugMessage(0, MAX_FLT, FColor::Green, FString::Printf(TEXT("Impulse:%f"), impulse));
	GEngine->AddOnScreenDebugMessage(1, MAX_FLT, FColor::Green, FString::Printf(TEXT("Period:%d"), period));
}

// Called every frame
void ABallGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fps += 1.0f;

	//Create Exploded Balls
	AExplodedBalls* NewBall = nullptr;
	if (int(fps) % period == 0 && Balls != nullptr)
	{
		if (Exploded && Exploded->Template)
		{
			Exploded->ToggleActive();
		}
		if (Exploded_Sound && Exploded_Sound->Sound)
		{
			Exploded_Sound->Play();
		}
		NewBall = generate(Balls, MyLocation * FVector(1, 1, 0) + GetActorRightVector() * 200.0f);
		NewBall->Ball->AddImpulse(GetActorRightVector() * impulse * 200.0f);
	}
}

//Generator Fuction, called when ball is generating
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

//PawnMovementComponent, get MovementComponent when required
UPawnMovementComponent* ABallGenerator::GetMovementComponent() const
{
	
	return MovementComponent;
}

//Player Inputs
void ABallGenerator::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("FoB", this, &ABallGenerator::Move_Foward);
	InputComponent->BindAxis("LR", this, &ABallGenerator::Move_Right);
	InputComponent->BindAxis("LR_Turn", this, &ABallGenerator::Turn_Right);
	InputComponent->BindAxis("Key_Updown", this, &ABallGenerator::Increase_Impulse);

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

	impulse += AxisValue * updown_speed;
	if (impulse < 0)impulse = 0;
	if (impulse > 60000)impulse = 60000;
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
