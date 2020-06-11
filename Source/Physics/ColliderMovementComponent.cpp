/*
@Project: Physics
@Owner: Donspaces
Date: Jun 4th, 2020 2:12 PM
*/


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Check Conditions
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{

		return;
	}

	FVector Movements = ConsumeInputVector() * DeltaTime;

	if (!Movements.IsNearlyZero())
	{

		FHitResult Hit;
		SafeMoveUpdatedComponent(Movements, UpdatedComponent->GetComponentRotation(), true, Hit);
		
	}
}