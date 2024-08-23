// Copyright: Nacho Ruiz


#include "Characters/SlashAnimInstance.h"

#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (CharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
		IsFalling = CharacterMovement->IsFalling();
		CharacterWeaponState = Character->GetCharacterWeaponState();
		if (const AWeapon* Weapon = Character->GetEquippedWeapon())
		{
			ShouldUseLeftHandFABRIK = Weapon->ShouldUseLeftHandFABRIK;
			LeftHandFABRIKTarget = Weapon->GetLeftHandSocketTarget(Character->GetMesh());
		}
	}
}
