// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PatrollTask.generated.h"

/**
 * 
 */
UCLASS()
class DOOM_API UBTTask_PatrollTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_PatrollTask(const FObjectInitializer& ObjectInitializer);

	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	int32 Index = 0;
	
};
