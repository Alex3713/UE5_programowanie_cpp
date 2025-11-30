// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5_cpp/components/MyAttributesComponent.h"
#include "UE5_cpp/interfaces/MyCombatInterface.h"

// Sets default values for this component's properties
UMyAttributesComponent::UMyAttributesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Health = MaxHealth;
	Stamina = MaxStamina;
}

// Called when the game starts
void UMyAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = FMath::Clamp(Health, 0.f, MaxHealth);
	Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
}

// Called every frame
void UMyAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RegenerateStamina(DeltaTime);
}

void UMyAttributesComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	
	OnHealthChanged.Broadcast(Health, MaxHealth);
	
	if (Health <= 0.f)
	{
		HandleDeath();
	}
}

void UMyAttributesComponent::SetMaxHealth(float NewMaxHealth, bool bResetHealth)
{
	MaxHealth = FMath::Max(NewMaxHealth, 0.f);

	if (bResetHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health = FMath::Clamp(Health, 0.f, MaxHealth);
	}
	OnHealthChanged.Broadcast(Health, MaxHealth);
	
	if (Health <= 0.f)
	{
		HandleDeath();
	}
}

void UMyAttributesComponent::ApplyDamage(float DamageAmount)
{
	if (DamageAmount <= 0.f || Health <= 0.f) return;
	SetHealth(Health - DamageAmount);
	if (Health <= 0.f)
	{
		HandleDeath();
	}
}

void UMyAttributesComponent::HandleDeath()
{
	OnDeath.Broadcast();
	if (AActor* Owner = GetOwner())
	{
		if (Owner->GetClass()->ImplementsInterface(UMyCombatInterface::StaticClass()))
		{
			IMyCombatInterface::Execute_OnDeath(Owner);
		}
	}
}

void UMyAttributesComponent::SetStamina(float NewValue)
{
	Stamina = FMath::Clamp(NewValue, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

void UMyAttributesComponent::SetMaxStamina(float NewMaxValue, bool bResetMaxValue)
{
	MaxStamina = FMath::Max(NewMaxValue, 0.f);
	if (bResetMaxValue)
	{
		Stamina = MaxStamina;
	}
	else
	{
		Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
	}
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);
}

bool UMyAttributesComponent::CanPayStaminaCost(float Cost) const
{
	return Stamina >= Cost;
}

void UMyAttributesComponent::PayStamina(float Cost)
{
	SetStamina(Stamina - Cost);
}

void UMyAttributesComponent::RegenerateStamina(float DeltaTime)
{
	if (Stamina < MaxStamina)
	{
		float Regen = StaminaSettings.StaminaRegenRate * DeltaTime;
		SetStamina(Stamina + Regen);
	}
}