// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
