// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE5_cpp/components/MyAttributesComponent.h"
#include "UE5_cpp/interfaces/MyCombatInterface.h"
#include "MyDestructibleObject.generated.h"

UCLASS()
class UE5_CPP_API AMyDestructibleObject : public AActor, public IMyCombatInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDestructibleObject();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMyAttributesComponent* Attributes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(FVector HitLocation, AActor* InstigatorActor, float DamageAmount) override;
	virtual void OnDeath_Implementation() override;
};
