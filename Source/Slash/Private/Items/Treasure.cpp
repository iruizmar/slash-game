// Copyright: Nacho Ruiz


#include "Items/Treasure.h"

#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	if (const ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor); !Character) { return; }

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
	}
	Destroy();
}
