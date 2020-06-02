// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplodedBalls.generated.h"

UCLASS()
class PHYSICS_API AExplodedBalls : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
			UStaticMeshComponent* Ball;
		
	UPROPERTY(EditAnywhere, Category = "Force")
		float explode_strength = 2000.0f;
	UPROPERTY(EditAnywhere, Category = "Force")
		float impulse = 2000.0f;

	float fps = 0.0f;
public:	
	// Sets default values for this actor's properties
	AExplodedBalls();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		AExplodedBalls* clone(FVector Location, FRotator Rotation = FRotator(0));
};
