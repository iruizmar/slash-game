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
	void ShowTrail() const;
	void HideTrail() const;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	bool ShouldUseLeftHandFABRIK = false;

	UFUNCTION(BlueprintCallable)
	FVector GetLeftHandSocketTarget(USkinnedMeshComponent* CharacterMesh) const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);

private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundBase* PickSound = nullptr;

	//section Animation montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* SheatheUnsheatheMontage = nullptr;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* HitTraceStartPoint;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* HitTraceEndPoint = nullptr;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* TrailParticleSystem = nullptr;

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

public:
	FORCEINLINE UAnimMontage* GetAttackMontage() const { return AttackMontage; }
	FORCEINLINE UAnimMontage* GetSheatheUnsheatheMontage() const { return SheatheUnsheatheMontage; }
};
