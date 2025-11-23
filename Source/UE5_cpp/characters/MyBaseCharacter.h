// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE5_cpp/interfaces/MyCombatInterface.h"
#include "UE5_cpp/components/MyAttributesComponent.h"
#include "UE5_cpp/enums/MyPawnState.h"
#include "Animation/AnimMontage.h"
#include "MyBaseCharacter.generated.h"

UCLASS()
class UE5_CPP_API AMyBaseCharacter : public ACharacter, public IMyCombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyBaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMyAttributesComponent* Attributes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EPawnState PawnState = EPawnState::Idle;

	virtual void SetPawnState(EPawnState NewState);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animations")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Animations")
	UAnimMontage* DeathMontage;

	void PlayHitReactMontage(const FVector& HitDirection);

public:	
	UFUNCTION(BlueprintCallable, Category = "State")
	FORCEINLINE bool IsOccupied() const
	{
		return PawnState == EPawnState::Occupied;
	}
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void GetHit_Implementation(FVector HitLocation, AActor* InstigatorActor, float DamageAmount) override;
	virtual void OnDeath_Implementation() override;
};
