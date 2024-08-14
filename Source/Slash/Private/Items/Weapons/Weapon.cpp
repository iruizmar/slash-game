// Copyright: Nacho Ruiz


#include "Items/Weapons/Weapon.h"

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName) const
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), InSocketName);
}
