// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HumanAI.generated.h"

UCLASS()
class FPSMOD1_API AHumanAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHumanAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool PlayerDetected;  // Allows AI to detect player
	bool CanAttackPlayer;  // Allows AI to attack player

	UPROPERTY(BlueprintReadWrite)
		bool CanDealDamage;  // Allows AI attack to damage player if it hits

	class AFPSMod1Character* PlayerREF;  // references the character player class

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerCollisionDetection;

	UPROPERTY(EditAnywhere)
		class USphereComponent* PlayerAttackCollisionDetection;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* DamageCollision;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> FireDamageType;

	UPROPERTY(EditAnywhere)
		AActor* MyCharacter;

	UPROPERTY(EditAnywhere)
		FHitResult MyHit;

	/* Health variables for AI*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float PreviousHealth;

	FTimerHandle FireTimerHandle;
	FTimerHandle MemberTimerHandle;

	class AHumanAIController* HumanAIController;

	void OnAIMoveCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	UPROPERTY(EditAnywhere)
		float StoppingDistance = 100.0f;

	FTimerHandle SeekPlayerTimerHandle;

	/** Get Health */
	UFUNCTION(BlueprintPure, Category = Health)
		float GetHealth();

	/** Update Health */
	UFUNCTION(BlueprintCallable, Category = Health)
		void UpdateHealth(float HealthChange);

	/** Damage Timer */
	UFUNCTION()
		void DamageTimer();

	/** Set Damage State */
	UFUNCTION()
		void SetDamageState();

	UFUNCTION()
		void MoveToPlayer();

	UFUNCTION()
		void SeekPlayer();

	UFUNCTION()
		void StopSeekingPlayer();

	UFUNCTION()
		void OnPlayerDetectedOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerDetectedOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnDealDamageOverlapEnd(class UPrimitiveComponent* OverlappedComp, 
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere) // Animation instances
		class UAnimMontage* EnemyAttackAnimation;

	class UAnimInstance* AnimInstance;

	UFUNCTION(BlueprintCallable)  // Calls animation instances
		void AttackAnimationEnded();

	UFUNCTION()
		void ApplyFireDamage();

};
