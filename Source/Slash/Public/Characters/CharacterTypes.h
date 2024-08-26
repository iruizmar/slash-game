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

UENUM(BlueprintType)
enum class EDeathPose: uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death1 UMETA(DisplayName = "Death 1"),
	EDP_Death2 UMETA(DisplayName = "Death 2"),
	EDP_Death3 UMETA(DisplayName = "Death 3"),
	EDP_Death4 UMETA(DisplayName = "Death 4"),
	EDP_Death5 UMETA(DisplayName = "Death 5"),
};
