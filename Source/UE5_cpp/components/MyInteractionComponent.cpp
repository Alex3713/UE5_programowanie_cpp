// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInteractionComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UMyInteractionComponent::UMyInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyInteractionComponent::PickUp(AActor* Item)
{
	if (!Item)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickUp() called with nullptr!"));
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickUp() failed: no owner found."));
		return;
	}

	Item->SetActorEnableCollision(false);

	Item->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	UE_LOG(LogTemp, Log, TEXT("%s picked up %s"), *Owner->GetName(), *Item->GetName());
}

void UMyInteractionComponent::TryInteract()
{
	FHitResult Hit;
	const FVector Start = GetComponentLocation();
	const FVector End = Start + GetForwardVector() * TraceDistance;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractTrace), false, GetOwner());
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel, Params);

#if !(UE_BUILD_SHIPPING)
	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 1.0f, 0, 1.5f);
	if (bHit) DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 8.f, FColor::Green, false, 1.0f);
#endif

	if (!bHit || !Hit.GetActor())
	{
		UE_LOG(LogTemp, Verbose, TEXT("TryInteract: nothing hit"));
		return;
	}
	PickUp(Hit.GetActor());
}