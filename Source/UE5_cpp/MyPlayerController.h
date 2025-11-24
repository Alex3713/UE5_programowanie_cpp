// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UMainHUDWidget;

UCLASS()
class UE5_CPP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) void TryInteract();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UMainHUDWidget> MainHUDWidgetClass;

	UPROPERTY() UMainHUDWidget* MainHUDWidget;

	UFUNCTION() void InitHUDWidget();
};
