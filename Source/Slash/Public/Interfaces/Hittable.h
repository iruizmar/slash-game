// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHittable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IHittable
{
	GENERATED_BODY()

public:
	virtual void GetHit(const FVector& ImpactPoint) = 0;
};
