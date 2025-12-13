#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyEnemyAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;
class UBlackboardData;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class UE5_CPP_API AMyEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyEnemyAIController();
	
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBlackboardData> BlackboardAsset;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	// ----- BB keys -----
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName Key_TargetActor = TEXT("TargetActor");
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName Key_CanSeePlayer = TEXT("CanSeePlayer");
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName Key_LastKnownPlayerLocation = TEXT("LastKnownPlayerLocation");
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName Key_NextPatrolPoint = TEXT("NextPatrolPoint");
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName Key_PawnState = TEXT("PawnState");
	UPROPERTY(EditDefaultsOnly, Category = "AI|Blackboard")
	FName Key_IsDead = TEXT("IsDead");
};
