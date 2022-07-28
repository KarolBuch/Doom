// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"

void ABaseEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
   
    
    if (AIBehavior != nullptr)
    {

        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
        
    }
    
}

void ABaseEnemyAIController::Tick(float DeltaSeconds)
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    float Distance = FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation());
    //SetFocus(PlayerPawn);
    UpdateControlRotation(0.1f);

    LocationKeyId = GetBlackboardComponent()->GetKeyID("PatrolPathVector");

    if (LineOfSightTo(PlayerPawn) && Distance < 3000.f)
    {
        MoveToActor(PlayerPawn, 50);
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
        
    }
   else
    {
     //ClearFocus(EAIFocusPriority::Gameplay);
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));

    }
    

}


 



