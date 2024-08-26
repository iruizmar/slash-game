// Copyright: Nacho Ruiz


#include "Enemies/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/StatsComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
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
	PlayHitAnimation(ImpactPoint);
	PlayHitSound(ImpactPoint);
	SpawnBloodEmitter(ImpactPoint);
}

float AEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	StatsComponent->ReceiveDamage(Damage);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercentage(StatsComponent->GetHealthPercentage());
	}

	return Damage;
}

void AEnemy::PlayHitAnimation(const FVector& ImpactPoint) const
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

void AEnemy::SpawnBloodEmitter(const FVector& ImpactPoint) const
{
	if (BloodParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, BloodParticle, ImpactPoint);
	}
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

void AEnemy::PlayHitSound(const FVector& ImpactPoint) const
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}
