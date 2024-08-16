// Copyright: Nacho Ruiz


#include "Characters/SlashCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
				UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())
		)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Interact);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(ToggleWeaponAction, ETriggerEvent::Triggered, this,
		                                   &ASlashCharacter::ToggleWeapon);
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != ECharacterActionState::ECAS_Unoccupied) { return; }
	const FVector2d MovementVector = Value.Get<FVector2d>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FRotationMatrix RotationMatrix = FRotationMatrix(YawRotation);

	const FVector ForwardDirection = RotationMatrix.GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = RotationMatrix.GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisValue = Value.Get<FVector2d>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ASlashCharacter::Jump()
{
	if constexpr (true)
	{
		Super::Jump();
	}
}

void ASlashCharacter::Interact()
{
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		OverlappingWeapon->Equip(GetMesh(), FName("hand_rSocket"));
		WeaponState = ECharacterWeaponState::ECWS_OneHandedWeapon;
		EquippedWeapon = OverlappingWeapon;
		OverlappingItem = nullptr;
	}
}

void ASlashCharacter::Attack()
{
	if (ActionState != ECharacterActionState::ECAS_Unoccupied || WeaponState == ECharacterWeaponState::ECWS_Unequipped)
	{
		return;
	}
	PlayAttackMontage();
	UE_LOG(LogTemp, Warning, TEXT("Attack start"));
	ActionState = ECharacterActionState::ECAS_Attacking;
}

void ASlashCharacter::ToggleWeapon()
{
	if (ActionState != ECharacterActionState::ECAS_Unoccupied || !EquippedWeapon) { return; }
	ActionState = ECharacterActionState::ECAS_SheathingOrUnSheathing;
	switch (WeaponState)
	{
	case ECharacterWeaponState::ECWS_OneHandedWeapon:
		WeaponState = ECharacterWeaponState::ECWS_Unequipped;
		PlaySheatheUnsheatheMontage(FName("Sheathe"));
		break;
	default:
		WeaponState = ECharacterWeaponState::ECWS_OneHandedWeapon;
		PlaySheatheUnsheatheMontage(FName("Unsheathe"));
		break;
	}
}

void ASlashCharacter::PlayAttackMontage() const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && AttackMontage)
	{
		FName SectionName;
		AnimInstance->Montage_Play(AttackMontage);
		switch (FMath::RandRange(0, 1))
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		default:
			SectionName = FName("Attack2");
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ASlashCharacter::PlaySheatheUnsheatheMontage(const FName SectionName) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && SheatheUnsheatheMontage)
	{
		AnimInstance->Montage_Play(SheatheUnsheatheMontage);
		AnimInstance->Montage_JumpToSection(SectionName, SheatheUnsheatheMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack end"));
	ActionState = ECharacterActionState::ECAS_Unoccupied;
}

void ASlashCharacter::SheatheUnsheatheEnd()
{
	ActionState = ECharacterActionState::ECAS_Unoccupied;
}

void ASlashCharacter::SheatheWeapon()
{
	if (!EquippedWeapon) { return; }
	EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("spine_05Socket"));
}

void ASlashCharacter::UnsheatheWeapon()
{
	if (!EquippedWeapon) { return; }
	EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("hand_rSocket"));
}
