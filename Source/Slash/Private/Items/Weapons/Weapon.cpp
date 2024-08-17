// Copyright: Nacho Ruiz


#include "Items/Weapons/Weapon.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName InSocketName)
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), InSocketName);
}

void AWeapon::Equip(USceneComponent* InParent, const FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	State = EItemState::EIS_Equiped;
	if (PickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickSound, GetActorLocation());
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
