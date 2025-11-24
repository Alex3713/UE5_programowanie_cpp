#pragma once

#include "CoreMinimal.h"
#include "MyPawnState.generated.h"

UENUM(BlueprintType)
enum class EPawnState : uint8
{
	Idle 		UMETA(DisplayName = "Idle"),
	InCombat 	UMETA(DisplayName = "InCombat"),
	Occupied 	UMETA(DisplayName = "Occupied"),
	OutOfStamina UMETA(DisplayName = "Exhausted"),
	Dead 		UMETA(DisplayName = "Dead")
};