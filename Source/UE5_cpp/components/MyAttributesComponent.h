// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyAttributesComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	float Health = 0.f;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getters
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const { return Health; };
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const { return MaxHealth; };

	// Setters
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetHealth(float NewHealth);
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetMaxHealth(float NewMaxHealth, bool bResetHealth = false);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyDamage(float DamageAmount);

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnDeath OnDeath;
};
