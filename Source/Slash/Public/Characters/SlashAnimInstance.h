// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "Animation/AnimInstance.h"
#include "SlashAnimInstance.generated.h"

class UCharacterMovementComponent;
class ASlashCharacter;

UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	ASlashCharacter* Character;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category=Movement)
	bool IsFalling = false;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	ECharacterWeaponState CharacterWeaponState;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	bool ShouldUseLeftHandFABRIK;

	UPROPERTY(BlueprintReadOnly, Category="Weapon")
	FVector LeftHandFABRIKTarget;
};
