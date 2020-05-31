// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS(Blueprintable)
class PHYSICS_API AMyActor : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, Category = "Properties")
		bool enable_force = true;
	UPROPERTY(EditAnywhere, Category = "Properties")
		float Force = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Properties")
		bool enable_velocity = false;
	UPROPERTY(EditAnywhere, Category = "Properties")
		float Velocity = 500.0f;
	UPROPERTY(EditAnywhere, Category = "Properties")
		float Theta = 45.0f;
	UPROPERTY(EditAnywhere, Category = "Properties")
		bool Force_Cont = false;
	
	float cfp = 0.0f;
	float XDir = 1.0f;
	float YDir = 1.0f;
	int Collide_Response = NULL;
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
