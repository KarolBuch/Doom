// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemy.h"

void ABaseEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
   
    
    if (AIBehavior != nullptr)
    {

        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
    GetWorldTimerManager().SetTimer(TalkingDemon,this, &ABaseEnemyAIController::DemonScream, 5.f, true, 0.f);
    
}

void ABaseEnemyAIController::Tick(float DeltaSeconds)
{

    ABaseEnemy* ControlledCharacter = Cast<ABaseEnemy>(GetPawn());
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
   
    if (PlayerPawn)
    {
        float Distance = FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation());
        //SetFocus(PlayerPawn);
        UpdateControlRotation(0.1f);

        LocationKeyId = GetBlackboardComponent()->GetKeyID("PatrolPathVector");

        if (LineOfSightTo(PlayerPawn) && Distance < 2000.f)
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
    
    

}
bool ABaseEnemyAIController::IsDead() const
{
    ABaseEnemy* ControlledCharacter = Cast<ABaseEnemy>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }

    return true;
}
void ABaseEnemyAIController::DemonScream()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if(!PlayerPawn)
    {
        return;
    }

    float Distance = FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation());
    if (LineOfSightTo(PlayerPawn) && Distance < 3000.f)
    {
        UGameplayStatics::SpawnSoundAttached(DemonSpotSound, RootComponent, TEXT("Root Component"));
    }

}

 



