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
#include "TimerComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impulse")
		float impulse = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impulse")
		int period = 260;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		int score = 0;
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

	UUserWidget* ScoreBoard;
	TSubclassOf<UUserWidget> ScoreBoardClass;

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
	UFUNCTION(BlueprintCallable,Category="Gens")
	AExplodedBalls* generate(AActor* Actortype, FVector Location);

	//PawnMovementComponent, get MovementComponent when required
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "OnScreenMsg")
	void print()
	{
		GEngine->AddOnScreenDebugMessage(2, MAX_FLT, FColor::Blue, FString::Printf(TEXT("Score:%d"), score));
	}
	
	//Player Inputs
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move_Foward(float AxisValue);
	void Move_Backward(float AxisValue);
	void Move_Left(float AxisValue);
	void Move_Right(float AxisValue);
	void Turn_Left(float AxisValue);
	void Turn_Right(float AxisValue);
	void Increase_Impulse(float AxisValue);
	void Decrease_Impulse(float AxisValue);
	void Increase_Period();
	void Decrease_Period();
	
};
