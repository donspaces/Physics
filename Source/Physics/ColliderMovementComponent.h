/*
@Project: Physics
@Owner: Donspaces
Date: Jun 4th, 2020 2:12 PM
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ColliderMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICS_API UColliderMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
