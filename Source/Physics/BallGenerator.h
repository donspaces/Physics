/*
@Project: Physics
@Owner: Donspaces
Date: Jun 4th, 2020 2:12 PM
*/

#pragma once

#include "ExplodedBalls.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "ColliderMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Engine/DataTable.h"
#include "PhysicsGameModeBase.h"
#include "BallGenerator.generated.h"

USTRUCT(BlueprintType)
struct FUserConfig : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float move_config;
	UPROPERTY(EditAnywhere)
		float rotate_config;
	UPROPERTY(EditAnywhere)
		float updown_config;
};

UCLASS()
class PHYSICS_API ABallGenerator : public APawn
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
			UStaticMeshComponent* Generator;
		UPROPERTY(EditAnywhere)
			UBoxComponent* Box;
		UPROPERTY(VisibleAnywhere)
			UParticleSystemComponent* Exploded;
	

	UColliderMovementComponent* MovementComponent;

	UAudioComponent* Exploded_Sound;

	UDataTable* UserData;
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impulse")
		float impulse = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impulse")
		int period = 260;
	UPROPERTY(EditAnywhere, Category = "Keyboard Control")
		float move = 2000.0f;
	UPROPERTY(EditAnywhere, Category = "Keyboard Control")
		float rotate = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Keyboard Control")
		float updown_speed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "ClearBalls")
		float span = 26.0f;

	float fps = 0.0f;

	TArray<AActor*> BallGen;

	AActor* Balls;

	FVector MyLocation;
	FRotator MyRotation;

public:	
	// Sets default values for this actor's properties
	ABallGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Generator Fuction, called when ball is generating
	AExplodedBalls* generate(AActor* Actortype, FVector Location, FRotator Rotation = FRotator(0));

	//PawnMovementComponent, get MovementComponent when required
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	
	//Player Inputs
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move_Foward(float AxisValue);
	void Move_Right(float AxisValue);
	void Turn_Right(float AxisValue);

	UFUNCTION(BlueprintCallable,Category="Gen_func")
		void Increase_Impulse(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Gen_func")
		void Increase_Period();

	UFUNCTION(BlueprintCallable, Category = "Gen_func")
		void Decrease_Period();

	void Escape();
	
};
