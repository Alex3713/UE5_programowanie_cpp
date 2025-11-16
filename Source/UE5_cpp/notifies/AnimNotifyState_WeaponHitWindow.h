#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_WeaponHitWindow.generated.h"

UCLASS()
class UE5_CPP_API UAnimNotifyState_WeaponHitWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MesComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
