// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Hittable.h"
#include "Physics/Experimental/ChaosEventType.h"
#include "BreakableActor.generated.h"

class UCapsuleComponent;
class ATreasure;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHittable
{
	GENERATED_BODY()

public:
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;

private:
	UPROPERTY(EditAnywhere, Category = "Breakable properties")
	TArray<TSubclassOf<ATreasure>> ToSpawn;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* BreakSound;

	UFUNCTION()
	void OnBreak(const FChaosBreakEvent& BreakEvent);
};
