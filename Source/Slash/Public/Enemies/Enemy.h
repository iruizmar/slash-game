// Copyright: Nacho Ruiz

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/Hittable.h"
#include "Enemy.generated.h"

class USphereComponent;
class UWidgetComponent;
class UHealthBarComponent;
class UStatsComponent;
class UUStatsComponent;
class AAIController;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHittable
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 150.f;

	UPROPERTY()
	FTimerHandle PatrolTimer;
	void OnPatrolTimerFinish() const;

protected:
	virtual void BeginPlay() override;

	enum class EHitDirection: uint8
	{
		EHD_Front, EHD_Back, EHD_Right, EHD_Left
	};

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

private:
	UPROPERTY(VisibleAnywhere, Category="Stats")
	UStatsComponent* StatsComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Stats")
	UHealthBarComponent* HealthBarWidget = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	USphereComponent* VisibilityRadius = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Montages")
	UAnimMontage* HitReactMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Montages")
	UAnimMontage* DeathMontage = nullptr;

	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* HitSound = nullptr;

	UPROPERTY(EditAnywhere, Category="VFX")
	UParticleSystem* BloodParticle = nullptr;

	UPROPERTY()
	AAIController* EnemyController = nullptr;

	UPROPERTY()
	AActor* CombatTarget = nullptr;

	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	AActor* CurrentPatrolTarget = nullptr;

	UPROPERTY()
	int32 CurrentPatrolTargetIndex = 0;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolWaitMax = 10.f;

	void PlayHitSound(const FVector& ImpactPoint) const;
	void PlayHitAnimation(const FVector& ImpactPoint) const;
	void SpawnBloodEmitter(const FVector& ImpactPoint) const;
	void Die();


	void PlayHitReactMontage(const EHitDirection Direction) const;
	void PlayDeathMontage();

	UFUNCTION()
	void OnVisibilitySphereCollisionOverlapBegins(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnVisibilitySphereCollisionOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	bool IsActorInRange(const AActor* InActor, double AcceptanceRadius) const;

	UFUNCTION()
	void MoveToActor(const AActor* InActor) const;
};
