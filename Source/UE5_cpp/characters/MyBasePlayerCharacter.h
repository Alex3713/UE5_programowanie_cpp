// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "UE5_cpp/components/MyInteractionComponent.h"
#include "UE5_cpp/objects/MyItem.h"
#include "MyBasePlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UE5_CPP_API AMyBasePlayerCharacter : public AMyBaseCharacter
{
	GENERATED_BODY()
	
public:
	AMyBasePlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	UAnimMontage* PickupMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bAttackLockedByMontage = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bInputDisabled = false;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetInputDisabled(bool bDisable);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TryAttack();
	
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void StartPickup(AActor* TargetItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	AMyItem* EquippedWeapon = nullptr;

	UFUNCTION(BlueprintCallable, Category="Equipment")
	bool CanEquipWeapon() const { return EquippedWeapon == nullptr; }

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void EquipWeapon(AMyItem* Weapon);

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void UnequipWeapon();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnPickupMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

	void RestoreControl();

	void DoPickup();

private:
	UPROPERTY()
	UMyInteractionComponent* InteractionComp = nullptr;

	UPROPERTY()
	AActor* PendingItem = nullptr;

	bool bIsInteracting = false;
	bool bIsAttacking = false;
	int32 LastAttackIndex = -1;
};
