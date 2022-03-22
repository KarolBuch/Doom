// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class DOOM_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()
	

	protected:
	virtual void BeginPlay() override;

	private:

	FTimerHandle TurnDelay;

	void TurnToPawn();
};
