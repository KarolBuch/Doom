// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "AnimationCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "DoomGameModeBase.h"
//#include "DrawDebugHelpers.h"


void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	PlayerChar = Cast<AAnimationCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));




	GetWorldTimerManager().SetTimer(AttackTimer, this, &ABaseEnemy::CheckAttackCondition, AttackSpeed,true, 0.f);
	
}

void ABaseEnemy::Tick(float DeltaTime)
{
	
	if (PlayerChar)
	{
		float Distance = FVector::Dist(GetActorLocation(), PlayerChar->GetActorLocation());
		

		
		if (Distance >= RotateRange && Distance < 7000.f)
		{
			RotateActor(PlayerChar->GetActorLocation());
		}
		else {
			GetSprite()->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

		}
		
		
		
	}
	ChangeAnimation();
}

void ABaseEnemy::RotateActor(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - RootComponent->GetComponentLocation();
	
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	
	
	GetSprite()->SetRelativeRotation(LookAtRotation);
/*
if (LookAtRotation.Yaw > -135 && LookAtRotation.Yaw < -45)
	{
		GetSprite()->SetFlipbook(Flipbooks.Idle);
		
	}
	else if (LookAtRotation.Yaw > -45 && LookAtRotation.Yaw < 45)
	{
		GetSprite()->SetFlipbook(Flipbooks.rightIdle);
	}
	else if (LookAtRotation.Yaw > 45 && LookAtRotation.Yaw < 135)
	{
		GetSprite()->SetFlipbook(Flipbooks.BackIdle);
	}
	else 
	{
		GetSprite()->SetFlipbook(Flipbooks.LeftIdle);
	}


	

	UE_LOG(LogTemp, Warning, TEXT(" LookAtRotation = %f"), );
	*/
}





	//Kr�cenie si� wzgl�dem rotacji gracza i potwora (jeszcze musze to wymy�li�):)

	



float  ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{



		float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		DamageToApply = FMath::Min(Health, DamageToApply);

		Health -= DamageToApply;
		UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

		if (IsDead())
		{
			//DetachFromControllerPendingDestroy();
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ADoomGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ADoomGameModeBase>();
			DeadAnimation();
			bIsDead = true;
			if (GameMode != nullptr)
			{
				GameMode->PawnKilled(this);
			}
		}

	    return DamageToApply;

}

void ABaseEnemy::CheckAttackCondition()
{
	if (!bIsDead)
	{
	TArray<AActor*> IgnoredActors;
	
	
		IgnoredActors.Add(this);

		if (InAttackRange())
		{
			UGameplayStatics::ApplyRadialDamage(GetWorld(),20.f,GetActorLocation(),AttackRange, nullptr,IgnoredActors,this,nullptr,true);
		}
	}
	
}

bool ABaseEnemy::InAttackRange()
{
	if (PlayerChar)
	{
		float Distance = FVector::Dist(GetActorLocation(), PlayerChar->GetActorLocation());

		if (Distance <= AttackRange)
		{		
			GetSprite()->SetFlipbook(Flipbooks.Attack);
			return true;
		}
	}

	return false;
}


void ABaseEnemy::ChangeAnimation()
{
	if (!bIsDead)
	{
		if (this->GetVelocity().X && this->GetVelocity().Y != 0 && !InAttackRange())
		{
			GetSprite()->SetFlipbook(Flipbooks.Walking);
		}
		else if(!InAttackRange())
		{
			//GetSprite()->SetFlipbook(Flipbooks.Idle);
		}
	}


	
}

void ABaseEnemy::DeadAnimation()
{

	GetSprite()->SetFlipbook(Flipbooks.dying);
	if (bIsDead)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(DestroyTimerHandle,
		[this]()
		{

			Destroy();
		},
		1.7f, false);
}
bool ABaseEnemy::IsDead() const
{
	return Health <= 0;
}

