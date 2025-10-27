// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CPP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable) void TryInteract();
	
};
