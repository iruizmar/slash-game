// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

class UHealthBar;

UCLASS()
class SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void SetHealthPercentage(float Percentage);

private:
	UPROPERTY()
	UHealthBar* HealthBarWidget = nullptr;
};
