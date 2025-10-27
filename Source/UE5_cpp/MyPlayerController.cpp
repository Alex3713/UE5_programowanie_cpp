// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "components/MyInteractionComponent.h"

void AMyPlayerController::TryInteract()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryInteract: no Pawn possessed"));
		return;
	}

	UMyInteractionComponent* Interaction = MyPawn->FindComponentByClass<UMyInteractionComponent>();
	if (!Interaction)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryInteract: no Interaction component"));
		return;
	}

	Interaction->TryInteract();
}
