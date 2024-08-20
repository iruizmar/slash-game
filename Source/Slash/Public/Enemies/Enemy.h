// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Hittable.h"
#include "Enemy.generated.h"

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHittable
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	enum class EHitDirection: uint8
	{
		EHD_Front, EHD_Back, EHD_Right, EHD_Left
	};

	void PlayHitReactMontage(const EHitDirection Direction) const;

private:
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	UAnimMontage* HitReactMontage;
};
