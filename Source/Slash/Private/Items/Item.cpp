// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Slash/DebugMacros.h"

AItem::AItem() : VerticalAmplitude(0.25f), TimeConstant(5.f), RunningTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

float AItem::GetTransformedSine(const float Value) const
{
	return VerticalAmplitude * FMath::Sin(Value * TimeConstant);
}

float AItem::GetTransformedCosine(float Value) const
{
	return VerticalAmplitude * FMath::Cos(Value * TimeConstant);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
}

template <typename T>
T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
