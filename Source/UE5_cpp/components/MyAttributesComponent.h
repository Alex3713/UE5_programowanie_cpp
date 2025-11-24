// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UE5_cpp/structs/StaminaCost.h"
#include "MyAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, Current, float, Max);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_CPP_API UMyAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyAttributesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void HandleDeath();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes|Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Health", meta = (AllowPrivateAccess = "true"))
	float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes|Stamina", meta = (AllowPrivateAccess = "true"))
	float MaxStamina = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Stamina", meta = (AllowPrivateAccess = "true"))
	float Stamina = 0.f;
	UPROPERTY(EditAnywhere, Category="Attributes|Stamina")
	FStaminaCost StaminaSettings;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	float GetHealth() const { return Health; };
	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	float GetMaxHealth() const { return MaxHealth; };

	UFUNCTION(BlueprintCallable, Category = "Attributes|Stamina")
	float GetStamina() const { return Stamina; };
	UFUNCTION(BlueprintCallable, Category = "Attributes|Stamina")
	float GetMaxStamina() const { return MaxStamina; };

	const FStaminaCost& GetStaminaCost() const { return StaminaSettings; };

	// Setters
	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	void SetHealth(float NewHealth);
	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	void SetMaxHealth(float NewMaxHealth, bool bResetHealth = false);

	UFUNCTION(BlueprintCallable, Category = "Attributes|Stamina")
	void SetStamina(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "Attributes|Stamina")
	void SetMaxStamina(float NewMaxValue, bool bResetMaxValue = false);

	// Functional
	UFUNCTION(BlueprintCallable, Category = "Attributes|Health")
	void ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Attributes|Stamina")
	bool CanPayStaminaCost(float Cost) const;
	UFUNCTION(BlueprintCallable, Category = "Attributes|Stamina")
	void PayStamina(float Cost);
	
	// Delegates
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnDeath OnDeath;
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnStaminaChanged OnStaminaChanged;

private:
	void RegenerateStamina(float DeltaTime);
};
