// Copyright: Nacho Ruiz


#include "Components/StatsComponent.h"

UStatsComponent::UStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStatsComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
}

float UStatsComponent::GetHealthPercentage() const
{
	return Health / MaxHealth;
}


void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
