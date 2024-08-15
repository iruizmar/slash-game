// Copyright: Nacho Ruiz


#include "Items/Weapons/Weapon.h"

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), InSocketName);
	FloatingIsEnabled = false;
}
