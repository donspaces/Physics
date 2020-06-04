/*
@Project: Physics
@Owner: Donspaces
Date: Jun 2th, 2020 11:59 PM
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplodedBalls.generated.h"

UCLASS()
class PHYSICS_API AExplodedBalls : public AActor
{
	GENERATED_BODY()
	public:
		UPROPERTY(VisibleAnywhere)
			UStaticMeshComponent* Ball;

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
};

