// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "AnimationCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
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
		if (Distance <= RotateRange)
		{
			RotateActor(PlayerChar->GetActorLocation());
		}

	}

	ChangeAnimation();
}

void ABaseEnemy::RotateActor(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - RootComponent->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	GetSprite()->SetRelativeRotation(LookAtRotation);


}



	//Krêcenie siê wzglêdem rotacji gracza i potwora (jeszcze musze to wymyœliæ):)
	/*if (LookAtRotation.Yaw > 10)
	{
		GetSprite()->SetFlipbook(Flipbooks.Walking);

	}
	else if(LookAtRotation.Yaw > 90)
	{
		GetSprite()->SetFlipbook(Flipbooks.Idle);
	}
	else if (LookAtRotation.Yaw > 180)
	{
		GetSprite()->SetFlipbook(Flipbooks.Shot);
	}
	else
	{
		GetSprite()->SetFlipbook(Flipbooks.HideWeapon);
	}*/
	



float  ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);

	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
	if (Health <= 0)
	{
		DeadAnimation();
	}
	return DamageToApply;

}

void ABaseEnemy::CheckAttackCondition()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	if (InAttackRange())
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(),20.f,GetActorLocation(),AttackRange, nullptr,IgnoredActors,this,nullptr,true);

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
	if (this->GetVelocity().X && this->GetVelocity().Y != 0 && !InAttackRange())
	{
		GetSprite()->SetFlipbook(Flipbooks.Walking);
	}
	else if(!InAttackRange())
	{
		GetSprite()->SetFlipbook(Flipbooks.Idle);
	}
}

void ABaseEnemy::DeadAnimation()
{
	GetSprite()->SetFlipbook(Flipbooks.dying);

	GetWorldTimerManager().SetTimer(DestroyTimerHandle,
		[this]()
		{

			Destroy();
		},
		1.7f, false);
}

