// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExplodedBalls.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallGenerator.generated.h"

UCLASS()
class PHYSICS_API ABallGenerator : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
			UStaticMeshComponent* Generator;
	
	float fps = 0.0f;
public:	
	// Sets default values for this actor's properties
	ABallGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		AExplodedBalls* generate(FVector Location, FRotator Rotation = FRotator(0));
};
