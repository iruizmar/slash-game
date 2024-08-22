// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Hittable.h"
#include "BreakableActor.generated.h"

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHittable
{
	GENERATED_BODY()

public:
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;
};
