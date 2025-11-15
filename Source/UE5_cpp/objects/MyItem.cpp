// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AMyItem::AMyItem()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetEnableGravity(true);
	
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);

	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
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

}

void AMyItem::SetEquipped(bool bNewEquipped)
{
	bIsEquipped = bNewEquipped;
	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		if (bIsEquipped)
		{
			Prim->SetSimulatePhysics(false);
			Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
