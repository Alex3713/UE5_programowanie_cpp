#include "AnimNotifyState_WeaponHitWindow.h"
#include "UE5_cpp/characters/MyBasePlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_WeaponHitWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (AMyBasePlayerCharacter* Char = Cast<AMyBasePlayerCharacter>(Owner))
	{
		Char->SetWeaponHitboxActive(true);
	}
}

void UAnimNotifyState_WeaponHitWindow::NotifyEnd(USkeletalMeshComponent* MesComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MesComp) return;

	AActor* Owner = MesComp->GetOwner();
	if (AMyBasePlayerCharacter* Char = Cast<AMyBasePlayerCharacter>(Owner))
	{
		Char->SetWeaponHitboxActive(false);
	}
}
