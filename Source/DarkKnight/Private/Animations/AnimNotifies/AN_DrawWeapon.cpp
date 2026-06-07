// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_DrawWeapon.h"

#include "Components/DkEquipmentComponent.h"

void UAN_DrawWeapon::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UDkEquipmentComponent* EquipmentComponent =
		Cast<UDkEquipmentComponent>(MeshComp->GetOwner()->GetComponentByClass(UDkEquipmentComponent::StaticClass()));
	if (EquipmentComponent)
	{
		EquipmentComponent->DrawWeapon(WeaponTypeTag, SocketName);
	}
}
