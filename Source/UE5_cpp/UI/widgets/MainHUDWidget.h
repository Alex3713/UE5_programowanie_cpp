#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

enum class EPawnState : uint8;

UCLASS()
class UE5_CPP_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateHealth(float Current, float Max);
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdateStamina(float Current, float Max);
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void UpdatePawnState(EPawnState Current);
};