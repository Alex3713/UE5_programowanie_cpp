#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyMainHUD.generated.h"

enum class EPawnState : uint8;
class UMainHUDWidget;
enum class EMyPawnState;

UCLASS()
class UE5_CPP_API AMyMainHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateHealth(float Current, float Max);
	UFUNCTION()
	void UpdateStamina(float Current, float Max);
	UFUNCTION()
	void UpdatePawnState(EPawnState NewState);

	void SetHUDWidget(UMainHUDWidget* InWidget);
	
private:	
	UPROPERTY()
	UMainHUDWidget* HUDWidget;
};
