// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyBaseCharacter::AMyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UMyAttributesComponent>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetPawnState(EPawnState::Idle);
}

// Called every frame
void AMyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyBaseCharacter::GetHit_Implementation(FVector HitLocation, AActor* InstigatorActor, float DamageAmount)
{
	if (!Attributes||bIsDead) return;
	if (Attributes)
	{
		Attributes->ApplyDamage(DamageAmount);
	}

	UE_LOG(LogTemp, Warning, TEXT("%ls got hit at %s by %s"), *GetName(), *HitLocation.ToString(), *GetNameSafe(InstigatorActor));
	UE_LOG(LogTemp, Warning, TEXT("Damage dealt: %f"), DamageAmount);

	if (Attributes->GetHealth() > 0.f)
	{
		SetPawnState(EPawnState::Occupied);
		
		const FVector HitDirection = HitLocation - GetActorLocation();
		PlayHitReactMontage(HitDirection);
	}
}

void AMyBaseCharacter::OnDeath_Implementation()
{
	if (bIsDead || PawnState == EPawnState::Dead) return;
	bIsDead = true;
	SetPawnState(EPawnState::Dead);
	
	UE_LOG(LogTemp, Error, TEXT("%s died (BaseCharacter OnDeath)"), *GetName());

	if (AController* PC = GetController())
	{
		PC->UnPossess();
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->DisableMovement();
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (DeathMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}
}

void AMyBaseCharacter::PlayHitReactMontage(const FVector& HitDirection)
{
	if (bIsDead || PawnState == EPawnState::Dead || !HitReactMontage) return;

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(HitReactMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			if (!bIsDead) SetPawnState(EPawnState::InCombat);
		});
		AnimInstance->Montage_SetEndDelegate(EndDelegate, HitReactMontage);
	}
	else
	{
		SetPawnState(EPawnState::InCombat);
	}
	
	// if (!AnimInstance) return;
	//
	// const FVector Forward = GetActorForwardVector();
	// const FVector Right = GetActorRightVector();
	// const FVector Dir = HitDirection.GetSafeNormal2D();
	//
	// const float ForwardDot = FVector::DotProduct(Forward, Dir);
	// const float RightDot = FVector::DotProduct(Right, Dir);
	//
	// FName SectionName = FName("HitFront");
	//
	// if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
	// {
	// 	SectionName = (ForwardDot > 0.f) ? FName("HitFront") : FName("HitBack");
	// }
	// else
	// {
	// 	SectionName = (RightDot > 0.f) ? FName("HitRight") : FName("HitLeft");
	// }

	// AnimInstance->Montage_Play(HitReactMontage);
	// AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
}

void AMyBaseCharacter::SetPawnState(EPawnState NewState)
{
	if (PawnState == NewState) return;
	if (NewState != EPawnState::OutOfStamina || NewState != EPawnState::Dead)
	{
		PreviousPawnState = PawnState;
	}
	PawnState = NewState;
	OnPawnStateChanged.Broadcast(PawnState);
	UE_LOG(LogTemp, Log, TEXT("%s PawnState changed to %d"), *GetName(), (int32)PawnState);
}