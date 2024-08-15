#pragma once

UENUM(BlueprintType)
enum class ECharacterWeaponState: uint8
{
	ECWS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECWS_OneHandedWeapon UMETA(DisplayName = "One-handed weapon"),
	ECWS_TwoHandedWeapon UMETA(DisplayName = "Two-handed weapon"),
};
