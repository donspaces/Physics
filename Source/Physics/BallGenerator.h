// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExplodedBalls.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "BallGenerator.generated.h"

UCLASS()
class PHYSICS_API ABallGenerator : public APawn
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
			UStaticMeshComponent* Generator;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impulse")
		float impulse = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impulse")
		int period = 260;
	UPROPERTY(EditAnywhere, Category = "Keyboard Control")
		float move = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Keyboard Control")
		float rotate = 5.0f;
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
	AExplodedBalls* generate(AActor* Actortype, FVector Location, FRotator Rotation = FRotator(0));

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move_Foward(float AxisValue);
	void Move_Backward(float AxisValue);
	void Move_Left(float AxisValue);
	void Move_Right(float AxisValue);
	void Turn_Left(float AxisValue);
	void Turn_Right(float AxisValue);

	void Escape();
	
};
