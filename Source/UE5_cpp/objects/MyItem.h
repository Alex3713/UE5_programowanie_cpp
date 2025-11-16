// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MyItem.generated.h"

UENUM(BlueprintType)
enum EItemType : uint8
{
	Weapon UMETA(DisplayName="Weapon"),
	Other UMETA(DisplayName="Other")
};

UCLASS()
class UE5_CPP_API AMyItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyItem();
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly) UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TEnumAsByte<EItemType> ItemType = Other;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	bool bIsEquipped = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Combat")
	UBoxComponent* HitBox = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Combat")
	bool bHitboxActive = false;
	UFUNCTION(BlueprintCallable, Category = "Item|Combat")
	void SetHitboxActive(bool bActive);
	
	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetEquipped(bool bNewEquipped);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
