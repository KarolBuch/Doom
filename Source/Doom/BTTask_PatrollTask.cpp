// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrollTask.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "BaseEnemy.h"
#include "PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


UBTTask_PatrollTask::UBTTask_PatrollTask(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	NodeName = TEXT("Get Path Points");
	
}

EBTNodeResult::Type UBTTask_PatrollTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	const UBlackboardComponent* MyBlackBoard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!MyController || !MyBlackBoard) 
	{
		return EBTNodeResult::Failed;
	}
	ABaseEnemy* Chr = Cast<ABaseEnemy>(MyController->GetPawn());
	if (!Chr)
	{
		return EBTNodeResult::Failed;
	}
	APatrolPath* PathRef = Cast<APatrolPath>(Chr->PatrolPath);
	if (!PathRef || PathRef->Locations.Num() < 1)
	{
		return EBTNodeResult::Succeeded;
	}
	//SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("PatrolPathVector", PathRef->Locations[Index]);

	if (PathRef->Locations.Num() < 0)
	{
		return EBTNodeResult::Failed;
	}
	if (Index < PathRef->Locations.Num() - 1)
	{
		Index++;
		return EBTNodeResult::Succeeded;
	}
	Index = 0;
	return EBTNodeResult::Succeeded;
}