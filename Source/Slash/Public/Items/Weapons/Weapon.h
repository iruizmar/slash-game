// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class UBoxComponent;
class USoundBase;

/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const;
	void Equip(USceneComponent* InParent, FName InSocketName);
	void BeginHitting() const;
	void EndHitting();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundBase* PickSound = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* HitTraceStartPoint;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* HitTraceEndPoint;

	UFUNCTION()
	virtual void OnCollisionBoxOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	TArray<AActor*> IgnoreActorsOnHit;
};
