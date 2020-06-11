// This project has been created by donspace. All right reserved.


#include "MyBlueprintFunctionLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AActor* UMyBlueprintFunctionLibrary::CubeGen(AActor* Actortype, FVector Location, float range)
{
	
	AActor* OtherActor = Actortype;

	FActorSpawnParameters Parameters;

	Parameters.Template = Actortype;

	UWorld* World = Actortype->GetWorld();

	FVector NewLocation = Location + range * FMath::VRand() * FVector(1, 1, 0);

	if (IsValid(World))
	{
		//OtherActor = World->SpawnActor<class AActor>(Actortype->GetClass(), NewLocation, FRotator(0), Parameters);
	}

	return OtherActor;
}
