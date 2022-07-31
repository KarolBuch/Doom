// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/COntroller.h"
#include "BaseEnemyAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	UE_LOG(LogTemp, Warning, TEXT("a pawn was killed"));

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	for (ABaseEnemyAIController* Controller : TActorRange<ABaseEnemyAIController>(GetWorld()))
	{
		if (!Controller->IsDead())
		{
			UE_LOG(LogTemp, Warning, TEXT("lol"));
			return;
		}
	}

	EndGame(true);

}
void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}