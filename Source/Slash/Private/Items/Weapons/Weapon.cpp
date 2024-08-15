// Copyright: Nacho Ruiz


#include "Items/Weapons/Weapon.h"

void AWeapon::Equip(USceneComponent* InParent, const FName InSocketName)
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), InSocketName);
	State = EItemState::EIS_Equiped;
}
