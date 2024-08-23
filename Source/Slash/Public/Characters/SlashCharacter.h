// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "SlashCharacter.generated.h"

class AWeapon;
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

	//section Input
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* MappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* LookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* JumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* InteractAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* AttackAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ToggleWeaponAction = nullptr;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void Interact();
	void Attack();
	void ToggleWeapon();

	//section Animation montages
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* SheatheUnsheatheMontage = nullptr;

private:
	UPROPERTY(VisibleInstanceOnly, Category = State)
	ECharacterWeaponState WeaponState = ECharacterWeaponState::ECWS_Unequipped;
	UPROPERTY(VisibleInstanceOnly, Category = State)
	ECharacterActionState ActionState = ECharacterActionState::ECAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair = nullptr;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem = nullptr;

	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedWeapon = nullptr;

	//section Play montage functions
	void PlayAttackMontage() const;
	void PlaySheatheUnsheatheMontage(FName SectionName) const;

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void SheatheUnsheatheEnd();
	UFUNCTION(BlueprintCallable)
	void SheatheWeapon();
	UFUNCTION(BlueprintCallable)
	void UnsheatheWeapon();
	UFUNCTION(BlueprintCallable)
	void BeginAttack() const;
	UFUNCTION(BlueprintCallable)
	void EndAttack() const;
	UFUNCTION(BlueprintCallable)
	void ShowWeaponTrail() const;
	UFUNCTION(BlueprintCallable)
	void HideWeaponTrail() const;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterWeaponState GetCharacterWeaponState() const { return WeaponState; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
};
