// This project has been created by donspace. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICS_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UPROPERTY()
		int args=0;
	

public:
	UFUNCTION(BlueprintCallable, Category = "Gens")
	AActor* CubeGen(AActor* Actortype, FVector Location, float range);
};
