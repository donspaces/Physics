// This project has been created by donspace. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DestructibleComponent.h"
#include "DestructibleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Engine/DataTable.h"
#include "Engine/Engine.h"
#include "NewActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PHYSICS_API UNewActorComponent : public UDestructibleComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNewActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Gens")
	ADestructibleActor* CubeGen(AActor* Actortype, FVector Location, float range)
	{

		ADestructibleActor* OtherActor = nullptr;

		FActorSpawnParameters Parameters;

		Parameters.Template = Actortype;

		UWorld* World = GetWorld();

		FVector NewLocation = Location + FMath::RandRange(0.0f, range) * FMath::VRand() * FVector(1, 1, 0);



		if (IsValid(World))
		{
			OtherActor = World->SpawnActor<class ADestructibleActor>(Actortype->GetClass(), Parameters);
		}
		OtherActor->SetActorLocation(NewLocation);
		OtherActor->SetActorRotation(FRotator(0));
		return OtherActor;
	}
	
};
