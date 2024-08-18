// Copyright: Nacho Ruiz


#include "Items/Weapons/Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

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

void AWeapon::SetCollisionEnabledType(const ECollisionEnabled::Type InType) const
{
	if (!CollisionBox) { return; }
	CollisionBox->SetCollisionEnabled(InType);
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

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		HitTraceStartPoint->GetComponentLocation(),
		HitTraceEndPoint->GetComponentLocation(),
		FVector(5.f, 5.f, 5.f),
		HitTraceStartPoint->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);
}
