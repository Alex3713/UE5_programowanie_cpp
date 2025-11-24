#include "MyPlayerController.h"
#include "UE5_cpp/UI/MyMainHUD.h"
#include "UE5_cpp/UI/widgets/MainHUDWidget.h"
#include "components/MyInteractionComponent.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitHUDWidget();
}

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

void AMyPlayerController::InitHUDWidget()
{
	if (!MainHUDWidgetClass) return;

	MainHUDWidget = CreateWidget<UMainHUDWidget>(this, MainHUDWidgetClass);
	if (!MainHUDWidget) return;

	MainHUDWidget->AddToViewport();

	if (AMyMainHUD* HUD = Cast<AMyMainHUD>(GetHUD()))
	{
		HUD->SetHUDWidget(MainHUDWidget);
	}
}