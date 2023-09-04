// Charles Makela. 26 Nov 2022. Following guide from Awesometuts.com


#include "HumanAI.h"
#include "FPSMod1Character.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "HumanAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "AITypes.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AHumanAI::AHumanAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Collision Detection"));

	PlayerCollisionDetection->SetupAttachment(RootComponent);

	PlayerAttackCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player Attack Collision Detection"));

	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
}

// Called when the game starts or when spawned
void AHumanAI::BeginPlay()
{
	Super::BeginPlay();

	FullHealth = 600.0f;  // Create variables
	Health = FullHealth;
	HealthPercentage = 1.0f;
	SetCanBeDamaged(true);  // Changed to setter
	
	HumanAIController = Cast<AHumanAIController>(GetController());
	HumanAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject
	(this, &AHumanAI::OnAIMoveCompleted);

	PlayerCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,  // Creates Begin Overlap and End Overlap for Collision Detection
		&AHumanAI::OnPlayerDetectedOverlapBegin);

	PlayerCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
		&AHumanAI::OnPlayerDetectedOverlapEnd);

	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,  // Creates Begin Overlap and End Overlap for Attack Collision Detection
		&AHumanAI::OnPlayerAttackOverlapBegin);

	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
		&AHumanAI::OnPlayerAttackOverlapEnd);

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this,  // Creates the Damage Collision when connecting collisions
		&AHumanAI::OnDealDamageOverlapBegin);
	
	AnimInstance = GetMesh()->GetAnimInstance();  // Reference to animation instance

	CanDealDamage = false;
}

void AHumanAI::OnAIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (!PlayerDetected)
	{
		HumanAIController->RandomPatrol();  // If player is not detected, continue patrol
	}
	else if (PlayerDetected && CanAttackPlayer)  // If player is detected and can attack, attack player
	{
		StopSeekingPlayer();

		// attack player plays montage
		AnimInstance->Montage_Play(EnemyAttackAnimation);
	}
}

void AHumanAI::MoveToPlayer()  // Moves to player location
{
	HumanAIController->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);
}

void AHumanAI::SeekPlayer()  // Looks for the player
{
	if (PlayerREF != nullptr)  // Adds check for null pointer
	{
		MoveToPlayer();
		GetWorld()->GetTimerManager().SetTimer(SeekPlayerTimerHandle, this,
			&AHumanAI::SeekPlayer, 0.25f, true);
	}
}

void AHumanAI::StopSeekingPlayer()  // Stops seeking player
{
	GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTimerHandle);
}

void AHumanAI::OnPlayerDetectedOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<AFPSMod1Character>(OtherActor);
	if (PlayerREF)
	{
		PlayerDetected = true;
		SeekPlayer();
	}
}

void AHumanAI::OnPlayerDetectedOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerREF = Cast<AFPSMod1Character>(OtherActor);
	if (PlayerREF)
	{
		PlayerDetected = false;
		StopSeekingPlayer();
		HumanAIController->RandomPatrol();
	}
}

void AHumanAI::OnPlayerAttackOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<AFPSMod1Character>(OtherActor);
	if (PlayerREF)
	{
		CanAttackPlayer = true;
	}
}

void AHumanAI::OnPlayerAttackOverlapEnd(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerREF = Cast<AFPSMod1Character>(OtherActor);
	if (PlayerREF)
	{
		CanAttackPlayer = false;

		// stop the attack animation and chase the player
		AnimInstance->Montage_Stop(0.0f, EnemyAttackAnimation);

		SeekPlayer();
	}
}

void AHumanAI::OnDealDamageOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerREF = Cast<AFPSMod1Character>(OtherActor);
	if ((PlayerREF != nullptr) && (OtherComp != nullptr))
	{
		// deal damage to player
		UE_LOG(LogTemp, Warning, TEXT("Player Damaged"));
		MyCharacter = Cast<AFPSMod1Character>(OtherActor);
		MyHit = SweepResult;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &AHumanAI::ApplyFireDamage, 2.2f, false, 0.0f);
	}
}

void AHumanAI::OnDealDamageOverlapEnd(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	CanDealDamage = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void AHumanAI::ApplyFireDamage()
{

	UGameplayStatics::ApplyPointDamage(MyCharacter, 200.0f, GetActorLocation(), MyHit, nullptr, this, FireDamageType);
	CanDealDamage = false;
}

void AHumanAI::AttackAnimationEnded()
{
	if (CanAttackPlayer)
	{
		AnimInstance->Montage_Play(EnemyAttackAnimation);
	}
}

// Called every frame
void AHumanAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHumanAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AHumanAI::GetHealth()  // Function to return Health Percentage
{
	return HealthPercentage;
}

void AHumanAI::SetDamageState()
{
	SetCanBeDamaged(true);  // Replaced with setter
}

void AHumanAI::DamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AHumanAI::SetDamageState, 1.0f, false);  // Set invincibility to 1 second.
}

float AHumanAI::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	SetCanBeDamaged(false);  // Changed to setter
	UpdateHealth(-DamageAmount);
	DamageTimer();
	return DamageAmount;
}

void AHumanAI::UpdateHealth(float HealthChange)
{
	Health += HealthChange;
	Health = FMath::Clamp(Health, 0.0f, FullHealth);
	PreviousHealth = HealthPercentage;
	HealthPercentage = Health / FullHealth;
}