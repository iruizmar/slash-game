// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlashCharacter.generated.h"

class AItem;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UGroomComponent;

UENUM(BlueprintType)
enum class ECharacterWeaponState: uint8
{
	ECWS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECWS_OneHandWeapon UMETA(DisplayName = "One hand weapon"),
};

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

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void Interact();

private:
	ECharacterWeaponState State = ECharacterWeaponState::ECWS_Unequipped;

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
	void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
};
