// This project has been created by donspace. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerComponent.h"
#include "TimeTicker.generated.h"

UCLASS()
class PHYSICS_API ATimeTicker : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
		UTimerComponent* MyTimer;

	int ticks = 0.0f;
	
public:	
	// Sets default values for this actor's properties
	ATimeTicker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
