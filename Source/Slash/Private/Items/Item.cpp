// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Components/SphereComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
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
	if (!FloatingIsEnabled)
	{
		return;
	}
	const float ZOffset = FloatingAmplitude * FMath::Sin(GetGameTimeSinceCreation() * FloatingFrequency);
	AddActorWorldOffset(FVector(0.f, 0.f, ZOffset));
}

template <typename T>
T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red,
		                                 FString::Printf(TEXT("%1s overlap start"), *OtherActor->GetName()));
	}
}

void AItem::OnSphereOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30.f, FColor::Red,
		                                 FString::Printf(TEXT("%1s overlap end"), *OtherActor->GetName()));
	}
}
