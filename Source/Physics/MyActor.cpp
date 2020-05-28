// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	VisualMesh->SetupAttachment(RootComponent);

	UMaterial* ColorMaterial;
	ColorMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(TEXT("/Game/StarterContent/Materials/M_Brick_Clay_New.M_Brick_Clay_New")).Get();

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
		if (SphereVisualAsset.Succeeded())
		{
			VisualMesh->SetStaticMesh(SphereVisualAsset.Object);
			VisualMesh->SetWorldLocation(FVector(1530.0f, 2300.0f, -10.0f));
			VisualMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
			VisualMesh->SetMaterial(0, ColorMaterial);

			VisualMesh->SetSimulatePhysics(true);
			VisualMesh->BodyInstance.bOverrideMass = true;
			VisualMesh->BodyInstance.SetMassOverride(2.0f);
			VisualMesh->SetLinearDamping(0.1f);
			
	}
		Force = 200.0f;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	cfp = 0.0f;
	VisualMesh->OnComponentHit.AddDynamic(this, &AMyActor::OnHit);
	if (enable_force == true && enable_velocity == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Variable \"enable_force\" and \"enable_velocity\" can't both be true. If so, \"enable_force\" will be used in priority"));
	}
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cfp += 1.0f;
	if (cfp < 60.0f && Force_Cont==false)
	{
		if(enable_force==true)
		{
			VisualMesh->AddForce(FVector(-Force * FMath::Sin(Theta * PI / 180.0f), -Force * FMath::Cos(Theta * PI / 180.0f), 0), TEXT("Force1"), true);
		}
		else if (enable_velocity == true)
		{
			VisualMesh->SetPhysicsLinearVelocity(FVector(-Velocity * FMath::Sin(Theta * PI / 180.0f), -Velocity * FMath::Cos(Theta * PI / 180.0f), 0), false);
		}
	}
	else if (Force_Cont == true)
	{
		if (enable_force == true)
		{
			VisualMesh->AddForce(FVector(-XDir * Force * FMath::Sin(Theta * PI / 180.0f), -YDir * Force * FMath::Cos(Theta * PI / 180.0f), 0), TEXT("ForceC1"), true);
		}
		else if (enable_velocity == true)
		{
			VisualMesh->SetPhysicsLinearVelocity(FVector(-XDir * Velocity * FMath::Sin(Theta * PI / 180.0f), -YDir * Velocity * FMath::Cos(Theta * PI / 180.0f), 0), false);
		}
	}
}

void AMyActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->GetName() == TEXT("EastPlane") && OtherComp != NULL && Collide_Response != 1)
	{
		Collide_Response = 1;
		YDir = -YDir;
		UE_LOG(LogTemp, Log, TEXT("EastPlane Collided!"));
	}
	if (OtherActor->GetName() == TEXT("NorthPlane") && OtherComp != NULL && Collide_Response != 2)
	{
		Collide_Response = 2;
		XDir = -XDir;
		UE_LOG(LogTemp, Log, TEXT("NorthPlane Collided!"));
	}
	if (OtherActor->GetName() == TEXT("WestPlane") && OtherComp != NULL && Collide_Response != 3)
	{
		Collide_Response = 3;
		YDir = -YDir;
		UE_LOG(LogTemp, Log, TEXT("WestPlane Collided!"));
	}
	if (OtherActor->GetName() == TEXT("SouthPlane") && OtherComp != NULL && Collide_Response != 4)
	{
		Collide_Response = 4;
		XDir = -XDir;
		UE_LOG(LogTemp, Log, TEXT("SouthPlane Collided!"));
	}
}
