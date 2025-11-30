#include "MyMainHUD.h"
#include "widgets/MainHUDWidget.h"
#include "UE5_cpp/components/MyAttributesComponent.h"
#include "GameFramework/PlayerController.h"
#include "UE5_cpp/characters/MyBasePlayerCharacter.h"

void AMyMainHUD::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;
	
	APawn* PlayerPawn = PC->GetPawn();
	if (!PlayerPawn) return;

	UMyAttributesComponent* Attributes = PlayerPawn->FindComponentByClass<UMyAttributesComponent>();
	if (!Attributes) return;

	Attributes->OnHealthChanged.AddDynamic(this, &AMyMainHUD::UpdateHealth);
	Attributes->OnStaminaChanged.AddDynamic(this, &AMyMainHUD::UpdateStamina);
	

	UpdateHealth(Attributes->GetHealth(), Attributes->GetMaxHealth());
	UpdateStamina(Attributes->GetStamina(), Attributes->GetMaxStamina());
	
	if (AMyBasePlayerCharacter* Char = Cast<AMyBasePlayerCharacter>(PlayerPawn))
	{
		Char->OnPawnStateChanged.AddDynamic(this, &AMyMainHUD::UpdatePawnState);
		UpdatePawnState(Char->GetPawnState());
	}
}

void AMyMainHUD::SetHUDWidget(UMainHUDWidget* InWidget)
{
	HUDWidget = InWidget;
}

void AMyMainHUD::UpdateHealth(float Current, float Max)
{
	if (HUDWidget)
	{
		HUDWidget->UpdateHealth(Current, Max);
	}
}

void AMyMainHUD::UpdateStamina(float Current, float Max)
{
	if (HUDWidget)
	{
		HUDWidget->UpdateStamina(Current, Max);
	}
}

void AMyMainHUD::UpdatePawnState(EPawnState NewState)
{
	if (HUDWidget)
	{
		HUDWidget->UpdatePawnState(NewState);
	}
}