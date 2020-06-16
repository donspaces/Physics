// This project has been created by donspace. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "TimerComponent.generated.h"

/*
USTRUCT(BlueprintType)
struct Ftimenow
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Timenow")
	int hour;
	UPROPERTY(EditAnywhere, Category = "Timenow")
	int minute;
	UPROPERTY(EditAnywhere, Category = "Timenow")
	int second;

	bool timeup()
	{
		if (hour == 0 && minute == 0 && second == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	Ftimenow()
	{

	}
	Ftimenow(int h, int m, int s)
	{
		hour = h;
		minute = m;
		second = s;
	}
	Ftimenow &operator--()
	{
		second--;
		if (second < 0)
		{
			minute--;
			second = 59;
		}
		if (minute < 0)
		{
			hour--;
			minute = 59;
		}
		return *this;
	}
	FString print()
	{
		FString timecode;
		timecode = FString::Printf(TEXT("Time: %d:%d:%d"), hour, minute, second);
		return timecode;
	}
};
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PHYSICS_API UTimerComponent : public UActorComponent
{
	GENERATED_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "time")
	FTimespan span = FTimespan(0, 1, 30);
	FTimespan init = FTimespan(0);
	FTimespan now = FTimespan(0);

	FTimespan Time_Left = span;

public:	
	// Sets default values for this component's properties
	UTimerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BluePrintCallable, Category = "Time")
	bool is_timeup()
	{
		if (Time_Left <= FTimespan(0))
		{
			GEngine->ClearOnScreenDebugMessages();
			return true;
		}

		return false;

	}
	UFUNCTION(BluePrintCallable, Category = "Time")
	bool timer()
	{
		GEngine->AddOnScreenDebugMessage(5, MAX_FLT, FColor::Yellow, TEXT("Time Left: ") + Time_Left.ToString());
		now = FDateTime::Now().GetTimeOfDay();
		Time_Left = span - (now - init);

		return is_timeup();
	}
	UFUNCTION(BluePrintCallable, Category = "Time")
	void reset_clock_to_time()
	{
		span = Time_Left;
	}
	UFUNCTION(BluePrintCallable, Category = "Time")
	void init_clock()
	{
		init = FDateTime::Now().GetTimeOfDay();
		now = FDateTime::Now().GetTimeOfDay();
	}
};
