// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "SlashCharacter.generated.h"

class AItem;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UGroomComponent;

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling = false;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* MappingContext = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InteractAction = nullptr;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction = nullptr;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void Interact();
	void Attack();

	//section Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	UAnimMontage* AttackMontage = nullptr;

private:
	ECharacterWeaponState WeaponState = ECharacterWeaponState::ECWS_Unequipped;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterWeaponState GetCharacterWeaponState() const { return WeaponState; }
};
