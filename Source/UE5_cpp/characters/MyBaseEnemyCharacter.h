// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "MyBaseEnemyCharacter.generated.h"

class AMyItem;

UCLASS()
class UE5_CPP_API AMyBaseEnemyCharacter : public AMyBaseCharacter
{
	GENERATED_BODY()
public:
	AMyBaseEnemyCharacter();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	AMyItem* DefaultWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Enemy")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Enemy")
	float AttackRange = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Enemy")
	float AttackCD = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Enemy")
	bool bIsAttacking = false;

	float LastAttackTime = -100.f;

	void TryAttackPlayer();
	void EquipDefaultWeapon();
	bool CanSeePlayer() const;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	virtual void GetHit_Implementation(FVector HitLocation, AActor* InstigatorActor, float DamageAmount) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetEnemyWeaponHitboxActive(bool bActive);
};
