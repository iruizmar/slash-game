// Copyright: Nacho Ruiz


#include "Items/Weapons/Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/Hittable.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AWeapon::AWeapon()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	HitTraceStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HitTraceStartingPoint"));
	HitTraceStartPoint->SetupAttachment(GetRootComponent());
	HitTraceEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HitTraceEndingPoint"));
	HitTraceEndPoint->SetupAttachment(GetRootComponent());

	TrailParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticleSystem"));
	TrailParticleSystem->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnCollisionBoxOverlapBegin);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName InSocketName) const
{
	ItemMesh->AttachToComponent(InParent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), InSocketName);
}

void AWeapon::Equip(USceneComponent* InParent, const FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	State = EItemState::EIS_Equiped;
	if (PickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickSound, GetActorLocation());
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::BeginHitting() const
{
	if (!CollisionBox) { return; }
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}


void AWeapon::EndHitting()
{
	if (!CollisionBox) { return; }
	IgnoreActorsOnHit.Empty();
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void AWeapon::ShowTrail() const
{
	TrailParticleSystem->BeginTrails(FName("TrailStart"), FName("TrailEnd"), ETrailWidthMode_FromCentre, 1.0f);
}

void AWeapon::HideTrail() const
{
	TrailParticleSystem->EndTrails();
}

void AWeapon::OnCollisionBoxOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	FHitResult HitResult;

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		HitTraceStartPoint->GetComponentLocation(),
		HitTraceEndPoint->GetComponentLocation(),
		FVector(5.f, 5.f, 5.f),
		HitTraceStartPoint->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		IgnoreActorsOnHit,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	//Did we hit a hittable?
	if (IHittable* Hittable = Cast<IHittable>(HitResult.GetActor()))
	{
		Hittable->GetHit(HitResult.ImpactPoint);
		IgnoreActorsOnHit.AddUnique(HitResult.GetActor());

		CreateFields(HitResult.ImpactPoint);
	}
}

FVector AWeapon::GetLeftHandSocketTarget(USkinnedMeshComponent* CharacterMesh) const
{
	const FTransform WeaponLeftHandSocketTransform = ItemMesh->GetSocketTransform(FName("LeftHand"));
	FVector OutPosition;
	FRotator OutRotator;
	CharacterMesh->TransformToBoneSpace(
		FName("hand_r"),
		WeaponLeftHandSocketTransform.GetLocation(),
		FRotator::ZeroRotator,
		OutPosition,
		OutRotator
	);
	return OutPosition;
}
