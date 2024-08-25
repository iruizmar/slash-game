// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SLASH_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category= "Stats")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category= "Stats")
	float Health = MaxHealth;
};
