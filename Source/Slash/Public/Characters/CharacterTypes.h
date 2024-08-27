#pragma once

UENUM(BlueprintType)
enum class ECharacterWeaponState: uint8
{
	ECWS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECWS_OneHandedWeapon UMETA(DisplayName = "One-handed weapon"),
	ECWS_TwoHandedWeapon UMETA(DisplayName = "Two-handed weapon"),
};

UENUM(BlueprintType)
enum class ECharacterActionState: uint8
{
	ECAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECAS_Attacking UMETA(DisplayName = "Attacking"),
	ECAS_SheathingOrUnSheathing UMETA(DisplayName = "Toggling weapon"),
};
