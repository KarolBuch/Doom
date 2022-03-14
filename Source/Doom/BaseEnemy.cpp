// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "AnimationCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlayerChar = Cast<AAnimationCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));


}

void ABaseEnemy::Tick(float DeltaTime)
{
	
		float x = MaxMovementDisance.X * FMath::Sin(Frequency.X * RunningTime) + StartPosition.X;


		SetActorLocation(FVector(x, GetActorLocation().Y, GetActorLocation().Z));
		RunningTime += DeltaTime;
	
	

	if (PlayerChar)
	{
		float Distance = FVector::Dist(GetActorLocation(), PlayerChar->GetActorLocation());

		if (Distance <= RotateRange)
		{
			RotateActor(PlayerChar->GetActorLocation());
			ChangeFlipbook(PlayerChar->GetActorRotation());
			
		}
	}
}

void ABaseEnemy::RotateActor(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - RootComponent->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	GetSprite()->SetRelativeRotation(LookAtRotation);

	if (LookAtRotation.Yaw > 100)
	{
		GetSprite()->SetFlipbook(Flipbooks.Walking);

	}
	else
	{
		GetSprite()->SetFlipbook(Flipbooks.Idle);
	}
}

void ABaseEnemy::ChangeFlipbook(FRotator PoseToTarget)
{
	


}
