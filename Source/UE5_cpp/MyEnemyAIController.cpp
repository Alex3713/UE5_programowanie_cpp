#include "MyEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "enums/MyPawnState.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AMyEnemyAIController::AMyEnemyAIController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	SetPerceptionComponent(*PerceptionComp);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 80.f;
	SightConfig->SetMaxAge(2.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AMyEnemyAIController::OnPerceptionUpdate);
}

void AMyEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// init BB
	if (BlackboardAsset)
	{
		UBlackboardComponent* BBComp = nullptr;
		UseBlackboard(BlackboardAsset, BBComp);
	}

	// start BT
	if (BehaviorTreeAsset) RunBehaviorTree(BehaviorTreeAsset);

	// clear BB keys at start
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->ClearValue(Key_TargetActor);
		BB->SetValueAsBool(Key_CanSeePlayer, false);
		BB->ClearValue(Key_LastKnownPlayerLocation);
		BB->ClearValue(Key_NextPatrolPoint);
		BB->SetValueAsEnum(Key_PawnState, (uint8)EPawnState::Idle);
		BB->SetValueAsBool(Key_IsDead, false);
	}
}

void AMyEnemyAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;

	APawn* TargetPawn = Cast<APawn>(Actor);
	if (!TargetPawn || !TargetPawn->IsPlayerControlled()) return;
	
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		BB->SetValueAsObject(Key_TargetActor, Actor);
		BB->SetValueAsBool(Key_CanSeePlayer, true);
		BB->SetValueAsVector(Key_LastKnownPlayerLocation, Actor->GetActorLocation());
	}
	else
	{
		BB->SetValueAsBool(Key_CanSeePlayer, false);
		BB->SetValueAsVector(Key_LastKnownPlayerLocation, Actor->GetActorLocation());
		BB->ClearValue(Key_TargetActor);
	}
}

