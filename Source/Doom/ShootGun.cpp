// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootGun.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "ShootImpact.h"

// Sets default values
AShootGun::AShootGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	GunAnimation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AnimationComp"));
	GunAnimation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShootGun::BeginPlay()
{
	Super::BeginPlay();

	bIsShoot = false;
	
	
	
}

// Called every frame
void AShootGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ChangeAnimation();
}

void AShootGun::ChangeAnimation()
{

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	FVector OwnerVelocity = OwnerPawn->GetVelocity();
	if (bIsShoot == false)
	{
	
		if (OwnerVelocity.X && OwnerVelocity.Y != 0 && !bIsShootingMode)
		{
			GunAnimation->SetFlipbook(Flipbooks.Walking);
		}
		else if (OwnerVelocity.X == 0 || bIsShootingMode)
		{
			GunAnimation->SetFlipbook(Flipbooks.Idle);
		}
	}
	
	
}

void AShootGun::PullTriger()
{
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	//if (OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	//if (OwnerController == nullptr) return;
	
	

	FVector End = LocationPoint + RotationPoint.Vector() * MaxRange;

	

	FHitResult Hit;

	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, LocationPoint, End, ECollisionChannel::ECC_GameTraceChannel1);
	if (bSuccess)
	{
		FVector ShotDirection = -RotationPoint.Vector();
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Cyan, true);
		GetWorld()->SpawnActor<AShootImpact>(ImpactClass, Hit.Location, RotationPoint);
		FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
		AActor* HitActor = Hit.GetActor();
		if(HitActor != nullptr)
		{

			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
		

	}


	//Animation when shooting
	bIsShoot = true;
	GunAnimation->SetFlipbook(Flipbooks.Shot);

	GetWorldTimerManager().SetTimer(FireRateTimerHandle,
		[this]()
		{
			GunAnimation->SetFlipbook(Flipbooks.Idle);
			bIsShoot = false;
			bIsShootingMode = true;
		},
		0.25f, false);

	GetWorldTimerManager().SetTimer(ShootingModeTimer,
		[this]()
		{

			bIsShootingMode = false;
		},
		1.89f, false);


	
}

void AShootGun::SetArrowLocation(FVector ShootPointLocation, FRotator ShotPointRotation)
{
	LocationPoint = ShootPointLocation;
	RotationPoint = ShotPointRotation;
}
