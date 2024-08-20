// Copyright: Nacho Ruiz


#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	const FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	double HitAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetActorForwardVector(), ToHit)));


	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), ToHit);
	// If CrossProduct points down, this HitAngle is actually be negative
	if (CrossProduct.Z < 0)
	{
		HitAngle *= -1.0f;
	}
	EHitDirection HitDirection = EHitDirection::EHD_Back;
	if (HitAngle >= -45.f && HitAngle < 45.f)
	{
		HitDirection = EHitDirection::EHD_Front;
	}
	else if (HitAngle >= -135.f && HitAngle < -45.f)
	{
		HitDirection = EHitDirection::EHD_Left;
	}
	else if (HitAngle >= 45.f && HitAngle < 135.f)
	{
		HitDirection = EHitDirection::EHD_Right;
	}

	PlayHitReactMontage(HitDirection);
}

void AEnemy::PlayHitReactMontage(const EHitDirection Direction) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && HitReactMontage)
	{
		FName SectionName;
		switch (Direction)
		{
		case EHitDirection::EHD_Back:
			SectionName = "FromBack";
			break;
		case EHitDirection::EHD_Front:
			SectionName = "FromFront";
			break;
		case EHitDirection::EHD_Left:
			SectionName = "FromLeft";
			break;
		case EHitDirection::EHD_Right:
			SectionName = "FromRight";
			break;
		default: ;
		}
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}
