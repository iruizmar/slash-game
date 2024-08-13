// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine parameters")
	float VerticalAmplitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine parameters")
	float TimeConstant;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sine parameters")
	float RunningTime;

	UFUNCTION(BlueprintPure)
	float GetTransformedSine(float Value) const;

	UFUNCTION(BlueprintPure)
	float GetTransformedCosine(float Value) const;

	template <typename T>
	static T Avg(T First, T Second);

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
};
