// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ABaseEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    GetWorldTimerManager().SetTimer(TurnDelay, this, &ABaseEnemyAIController::TurnToPawn, 2.f, true, 0.f);
    
    //SetFocus(PlayerPawn);

}


void ABaseEnemyAIController::TurnToPawn()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    
    SetFocus(PlayerPawn);
    

}












 //GetWorldTimerManager().SetTimer(TurnDelay, this, &ABaseEnemyAIController::TurnToPawn, 5.f, true, 0.f);
    //SetFocus(PlayerPawn);

 



