// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating movement")
	float FloatingAmplitude = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating movement")
	bool FloatingIsEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating movement")
	float FloatingFrequency = 2.5f;

	UFUNCTION()
	virtual void OnSphereOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void OnSphereOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	void Float();
	template <typename T>
	static T Avg(T First, T Second);
};
