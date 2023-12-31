// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSMod1Character.h"
#include "HumanAI.h"
#include "FPSMod1GameMode.generated.h"

//enum to store the current state of gameplay
UENUM()
enum class EGamePlayState
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS(minimalapi)
class AFPSMod1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSMod1GameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	AFPSMod1Character* MyCharacter;

	/** Returns the current playing state */
	UFUNCTION(BlueprintPure, Category = Health)
		EGamePlayState GetCurrentState() const;

	/** Sets a new playing state */
	void SetCurrentState(EGamePlayState NewState);

private:
	/**Keeps track of the current playing state */
	EGamePlayState CurrentState;

	/**Handle any function calls that rely upon changing the playing state of our game */
	void HandleNewState(EGamePlayState NewState);
};



