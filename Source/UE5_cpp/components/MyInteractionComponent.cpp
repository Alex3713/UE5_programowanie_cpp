// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInteractionComponent.h"
#include "UE5_cpp/characters/MyBasePlayerCharacter.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"
#include "WorldCollision.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "UE5_cpp/objects/MyItem.h"
#include "Components/SceneComponent.h"

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

	if (AMyItem* MyItem = Cast<AMyItem>(Item))
	{
		if (MyItem->bIsEquipped)
		{
			return;
		}
		if (MyItem->ItemType == EItemType::Weapon)
		{
			if (AMyBasePlayerCharacter* Char = Cast<AMyBasePlayerCharacter>(Owner))
			{
				if (Char->CanEquipWeapon())
				{
					Char->EquipWeapon(MyItem);
					return;
				}
			}
		}
	}
	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Item->GetRootComponent()))
	{
		Prim->SetSimulatePhysics(false);
		Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Prim->SetCollisionResponseToAllChannels(ECR_Ignore);
		Prim->SetMobility(EComponentMobility::Movable); // just in case...
	}
	Item->SetActorEnableCollision(false);

	if (ACharacter* Char = Cast<ACharacter>(Owner))
	{
		if (USkeletalMeshComponent* Body = Char->GetMesh())
		{
			static const FName CarrySocket(TEXT("CarrySocket"));
			Item->AttachToComponent(Body, FAttachmentTransformRules::SnapToTargetNotIncludingScale, CarrySocket);
		}
		else
		{
			Item->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	else
	{
		Item->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
	}
	Item->SetActorHiddenInGame(true);
	Item->SetActorTickEnabled(false);
	// Item->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	UE_LOG(LogTemp, Log, TEXT("%s picked up %s"), *Owner->GetName(), *Item->GetName());
}

void UMyInteractionComponent::TryInteract()
{
	// FHitResult Hit;
	const FVector Center = GetComponentLocation();
	// const FVector End = Center + GetForwardVector() * TraceDistance;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractOverlap), false, GetOwner());

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	// ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	// ObjParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	TArray<FOverlapResult> Overlaps;
	const bool bAny = GetWorld()->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, ObjParams, FCollisionShape::MakeSphere(SphereRadius), Params);

#if !(UE_BUILD_SHIPPING)
	// DrawDebugLine(GetWorld(), Center, End, FColor::Cyan, false, 1.0f, 0, 1.0f);
	DrawDebugSphere(GetWorld(), Center, SphereRadius, 16, bAny ? FColor::Green : FColor::Red, false, 0.5f);
#endif

	if (!bAny)
	{
		UE_LOG(LogTemp, Verbose, TEXT("TryInteract: no Item hit in sphere"));
		return;
	}
	
	AActor* Best = nullptr;
	float BestScore = -FLT_MAX;
	const FVector Fwd = GetForwardVector().GetSafeNormal();

	for (const FOverlapResult& O : Overlaps) 
	{
		AActor* A = O.GetActor();
		if (!A) continue;
		if (A == GetOwner()) continue;
		if (A->GetAttachParentActor() == GetOwner()) continue;
		if (AMyItem* Item = Cast<AMyItem>(A))
		{
			if (Item->bIsEquipped) continue;
		}
		if (!A->IsA<AMyItem>()) continue;

		const FVector To = (A->GetActorLocation() - Center);
		const float Dist = To.Size();
		const float Align = FVector::DotProduct(Fwd, To.GetSafeNormal());
		const float Score = Align * 1000.f - Dist;
		
		if (Score > BestScore)
		{
			BestScore = Score;
			Best = A;	
		}
	}
	
	if (!Best)
	{
		UE_LOG(LogTemp, Verbose, TEXT("TryInteract: overlaps found, but no valid Item"));
		return;
	}

	if (Best)
	{
		if (AMyBasePlayerCharacter* Char = Cast<AMyBasePlayerCharacter>(GetOwner()))
		{
			Char->StartPickup(Best);
		}
		else
		{
			PickUp(Best);
		}
	}
}