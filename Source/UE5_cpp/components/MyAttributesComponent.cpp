// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5_cpp/components/MyAttributesComponent.h"
#include "UE5_cpp/interfaces/MyCombatInterface.h"

// Sets default values for this component's properties
UMyAttributesComponent::UMyAttributesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Health = MaxHealth;
}

// Called when the game starts
void UMyAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = FMath::Clamp(Health, 0.f, MaxHealth);
}

// Called every frame
void UMyAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyAttributesComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
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

	if (Health <= 0.f)
	{
		HandleDeath();
	}
}

void UMyAttributesComponent::ApplyDamage(float DamageAmount)
{
	if (DamageAmount <= 0.f || Health <= 0.f) return;

	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
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
