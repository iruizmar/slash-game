// Copyright: Nacho Ruiz


#include "Enemies/Enemy.h"

#include "Characters/SlashCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
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

	VisibilityRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Visibility radius"));
	VisibilityRadius->SetupAttachment(GetRootComponent());
	VisibilityRadius->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	VisibilityRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
	VisibilityRadius->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	VisibilityRadius->SetSphereRadius(500.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidget->SetVisibility(false);
	VisibilityRadius->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnVisibilitySphereCollisionOverlapBegins);
	VisibilityRadius->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnVisibilitySphereCollisionOverlapEnd);
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
	if (StatsComponent->GetIsAlive())
	{
		PlayHitAnimation(ImpactPoint);
	}
	PlayHitSound(ImpactPoint);
	SpawnBloodEmitter(ImpactPoint);
}

float AEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	StatsComponent->ReceiveDamage(Damage);
	HealthBarWidget->SetVisibility(true);
	HealthBarWidget->SetHealthPercentage(StatsComponent->GetHealthPercentage());
	CombatTarget = EventInstigator->GetPawn();
	if (!StatsComponent->GetIsAlive())
	{
		Die();
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

void AEnemy::Die()
{
	PlayDeathMontage();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	HealthBarWidget->SetVisibility(false);
	SetLifeSpan(15.f);
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

void AEnemy::PlayDeathMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(0, 5);
		FName SectionName;
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;

		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;

		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;

		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		default:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
}

void AEnemy::OnVisibilitySphereCollisionOverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                      bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor); !Character) { return; }

	if (StatsComponent->GetHealthPercentage() < 1.f)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::OnVisibilitySphereCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor); !Character) { return; }

	HealthBarWidget->SetVisibility(false);
}
