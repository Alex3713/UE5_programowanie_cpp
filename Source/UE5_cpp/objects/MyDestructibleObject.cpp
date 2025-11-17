// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5_cpp/objects/MyDestructibleObject.h"
#include "GameFramework/Actor.h"

// Sets default values
AMyDestructibleObject::AMyDestructibleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECC_Destructible);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel( ECC_GameTraceChannel1, ECR_Overlap);
	Mesh->SetGenerateOverlapEvents(true);
	
	Attributes = CreateDefaultSubobject<UMyAttributesComponent>(TEXT("Attributes"));
}

// Called when the game starts or when spawned
void AMyDestructibleObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyDestructibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyDestructibleObject::GetHit_Implementation(FVector HitLocation, AActor* InstigatorActor, float DamageAmount)
{
	if (Attributes) Attributes->ApplyDamage(DamageAmount);

	UE_LOG(LogTemp, Warning, TEXT("Damage dealt: %f"), DamageAmount);
}

void AMyDestructibleObject::OnDeath_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("Object destroyed"));
	Destroy();
}