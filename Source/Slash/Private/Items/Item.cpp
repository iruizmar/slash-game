// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Characters/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EmbersEffect"));
	EmbersEffect->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverlapEnd);
}

void AItem::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	Float();
}

void AItem::Float()
{
	if (!FloatingIsEnabled || State != EItemState::EIS_Floating)
	{
		return;
	}
	const float ZOffset = FloatingAmplitude * FMath::Sin(GetGameTimeSinceCreation() * FloatingFrequency);
	AddActorWorldOffset(FVector(0.f, 0.f, ZOffset));
}

void AItem::OnSphereOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor))
	{
		Character->SetOverlappingItem(this);
	}
}

void AItem::OnSphereOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (ASlashCharacter* Character = Cast<ASlashCharacter>(OtherActor))
	{
		Character->SetOverlappingItem(nullptr);
	}
}
