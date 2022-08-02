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
	
	public:
	virtual void Tick(float DeltaSeconds) override;
	bool IsDead() const;
	protected:
	virtual void BeginPlay() override;

	private:

	void DemonScream();

	FTimerHandle TurnDelay;

	FTimerHandle TalkingDemon;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)
		USoundBase* DemonSpotSound;



	uint8 EnemyKeyId;
	uint8 LocationKeyId;
	uint8 ContactKeyId;
	
};
