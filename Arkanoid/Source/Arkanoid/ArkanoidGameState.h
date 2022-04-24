// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ArkanoidGameState.generated.h"

/**
 * 
 */
UCLASS()
class ARKANOID_API AArkanoidGameState : public AGameStateBase
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Score = 0;

public:
	void IncrementScore(int delta);
};
