// This project has been created by donspace. All right reserved.


#include "TimeTicker.h"

// Sets default values
ATimeTicker::ATimeTicker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyTimer = CreateDefaultSubobject<UTimerComponent>(TEXT("MyTimer"));
	
}

// Called when the game starts or when spawned
void ATimeTicker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimeTicker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ticks += 1.0f;
	if (int(ticks) % int(1 / DeltaTime) == 0)
	{
		MyTimer->timer();
	}
}

