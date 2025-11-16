// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBasePlayerCharacter.h"
#include "UE5_cpp/components/MyInteractionComponent.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

AMyBasePlayerCharacter::AMyBasePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyBasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InteractionComp = FindComponentByClass<UMyInteractionComponent>();
}

void AMyBasePlayerCharacter::StartPickup(AActor* TargetItem)
{
	if (bIsInteracting || !TargetItem) return;

	bIsInteracting = true;
	PendingItem = TargetItem;

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move -> DisableMovement();
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(true);
	}

	UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (PickupMontage && Anim)
	{
		Anim->OnPlayMontageNotifyBegin.AddDynamic(this, &AMyBasePlayerCharacter::OnMontageNotifyBegin);

		Anim->Montage_Play(PickupMontage);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AMyBasePlayerCharacter::OnPickupMontageEnded);
		Anim->Montage_SetEndDelegate(EndDelegate, PickupMontage);
	}
	else
	{
		DoPickup();
		RestoreControl();
		bIsInteracting = false;
	}
}

void AMyBasePlayerCharacter::OnPickupMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	DoPickup();
	RestoreControl();
	bIsInteracting = false;

	if (UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		Anim->OnPlayMontageNotifyBegin.RemoveAll(this);
	}
}

void AMyBasePlayerCharacter::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& /*Payload*/)
{
	if (NotifyName == TEXT("PickupAttach"))
	{
		DoPickup();
	}
}

void AMyBasePlayerCharacter::DoPickup()
{
	if (!InteractionComp || !PendingItem) return;
	InteractionComp->PickUp(PendingItem);
	PendingItem = nullptr;
}

void AMyBasePlayerCharacter::RestoreControl()
{
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->SetMovementMode(MOVE_Walking);
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(false);
	}
}

void AMyBasePlayerCharacter::EquipWeapon(AMyItem* Weapon)
{
	if (!Weapon || !CanEquipWeapon()) return;

	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Weapon->GetRootComponent()))
	{
		Prim->SetSimulatePhysics(false);
		Weapon->SetActorEnableCollision(false);
	}

	static const FName WeaponSocket(TEXT("RightHandSocket"));
	if (USkeletalMeshComponent* Body = GetMesh())
	{
		Weapon->AttachToComponent(Body, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	Weapon->SetOwner(this);
	Weapon->SetEquipped(true);
	EquippedWeapon = Weapon;

	UE_LOG(LogTemp, Log, TEXT("Equipped weapon: %s"), *GetNameSafe(Weapon));
}

void AMyBasePlayerCharacter::UnequipWeapon()
{
	if (!EquippedWeapon) return;

	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(EquippedWeapon->GetRootComponent()))
	{
		EquippedWeapon->SetEquipped(false);
		Prim->SetSimulatePhysics(true);
		EquippedWeapon->SetActorEnableCollision(true);
	}

	UE_LOG(LogTemp, Log, TEXT("Unequipped weapon: %s"), *GetNameSafe(EquippedWeapon));
}

void AMyBasePlayerCharacter::SetInputDisabled(bool bDisable)
{
	bInputDisabled = bDisable;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(bDisable);
	}
	if (bDisable)
	{
		if (auto* Move = GetCharacterMovement()) Move->StopMovementImmediately();
	}
}

void AMyBasePlayerCharacter::TryAttack()
{
	if (bInputDisabled) return;
	if (!EquippedWeapon) return;
	if (bAttackLockedByMontage && bIsAttacking) return;
	if (AttackMontages.Num() == 0) return;

	UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!Anim) return;

	int32 Index = 0;
	if (AttackMontages.Num() == 1)
	{
		Index = 0;
	}
	else
	{
		do
		{
			Index = FMath::RandRange(0, AttackMontages.Num() - 1);
		}while (Index == LastAttackIndex);
	}
	UAnimMontage* Chosen = AttackMontages[Index];
	if (!Chosen)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryAttack: null montage at index %d"), Index);
		return;
	}

	SetInputDisabled(true);
	
	float PlayRes = Anim->Montage_Play(Chosen, 1.f);
	if (PlayRes <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryAttack: Montage_Play failed."));
		SetInputDisabled(false);
		return;
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMyBasePlayerCharacter::OnAttackMontageEnded);
	Anim->Montage_SetEndDelegate(EndDelegate, Chosen);

	bIsAttacking = true;
	LastAttackIndex = Index;
}

void AMyBasePlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetInputDisabled(false);
	bIsAttacking = false;
}
