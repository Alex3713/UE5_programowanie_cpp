// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyItem::AMyItem()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(Mesh);

	// // DEBUG CONFIG
	// HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// HitBox->SetCollisionObjectType(ECC_WorldDynamic);
	// HitBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HitBox->SetGenerateOverlapEvents(true);
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMyItem::OnHitBoxOverlap);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);

	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HitBox)
	{
		const FTransform T = HitBox->GetComponentTransform();
		DrawDebugBox(GetWorld(), T.GetLocation(), HitBox->GetScaledBoxExtent(), T.GetRotation(), bHitboxActive ? FColor::Red : FColor::Blue, false, -1.f, 0, 1.5f);
	}
}

void AMyItem::SetEquipped(bool bNewEquipped)
{
	bIsEquipped = bNewEquipped;
	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		if (bIsEquipped)
		{
			Prim->SetSimulatePhysics(false);
			Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		else
		{
			Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Prim->SetCollisionResponseToAllChannels(ECR_Ignore);
			Prim->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
			Prim->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
			Prim->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
			Prim->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			Prim->SetCollisionObjectType(ECC_GameTraceChannel1);
		}
	}
}

void AMyItem::SetHitboxActive(bool bActive)
{
	bHitboxActive = bActive;
	
	UE_LOG(LogTemp, Warning, TEXT("SetHitboxActive(%d) on %s"), bActive ? 1 : 0, *GetNameSafe(this));

	if (!HitBox) return;

	if (bHitboxActive)
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		HitBox->SetGenerateOverlapEvents(true);
	}
	else
	{
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HitBox->SetGenerateOverlapEvents(false);
	}
}

void AMyItem::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComp,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bHitboxActive) return;
	if (OtherActor == GetOwner()) return;

	UE_LOG(LogTemp, Log, TEXT("Weapon hit: %s"), *GetNameSafe(OtherActor));
}