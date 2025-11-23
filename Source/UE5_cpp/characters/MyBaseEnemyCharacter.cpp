#include "MyBaseEnemyCharacter.h"
#include "Components/CapsuleComponent.h"

AMyBaseEnemyCharacter::AMyBaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyBaseEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TryAttackPlayer();
}

bool AMyBaseEnemyCharacter::CanSeePlayer() const
{
	if (bIsDead) return false;

	UWorld* World = GetWorld();
	if (!World) return false;

	const float ZOffset = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f;
	const FVector Start = GetActorLocation() + FVector(0.f, 0.f, ZOffset);
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = World->LineTraceSingleByChannel( Hit, Start, End, ECollisionChannel::ECC_GameTraceChannel3, Params);
	FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(World, Start, End, LineColor, false, 0.1f, 0, 2.f);
	
	if (!bHit) return false;

	AActor* HitActor = Hit.GetActor();

	return HitActor && HitActor->ActorHasTag(FName("Player"));
}

void AMyBaseEnemyCharacter::TryAttackPlayer()
{
	if (IsOccupied() || bIsDead) return;

	const float Time = GetWorld()->GetTimeSeconds();
	if (Time - LastAttackTime < AttackCD) return;
	if (!CanSeePlayer()) return;
	if (!AttackMontage) return;

	UAnimInstance* Anim = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (!Anim) return;

	float PlayRes = Anim->Montage_Play(AttackMontage, 1.f);
	if (PlayRes <= 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy TryAttack: Montage_Play failed"));
		return;
	}

	SetPawnState(EPawnState::InCombat);
	bIsAttacking = true;
	LastAttackTime = Time;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject( this, &AMyBaseEnemyCharacter::OnAttackMontageEnded);
	Anim->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void AMyBaseEnemyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;

	if (!bIsDead && !IsOccupied()) SetPawnState(EPawnState::InCombat);
}

void AMyBaseEnemyCharacter::GetHit_Implementation(FVector HitLocation, AActor* InstigatorActor, float DamageAmount)
{
	if (bIsAttacking)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			if (AttackMontage) Anim->Montage_Stop(0.1f, AttackMontage);
		}
		bIsAttacking = false;
	}
	SetPawnState(EPawnState::Occupied);

	Super::GetHit_Implementation(HitLocation, InstigatorActor, DamageAmount);
}