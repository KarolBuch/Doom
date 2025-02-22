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

	MagazineAmmo = 30;
	
	
}

// Called every frame
void AShootGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//ChangeAnimation();
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
			//GunAnimation->SetFlipbook(Flipbooks.Walking);
		}
		else if (OwnerVelocity.X == 0 || bIsShootingMode)
		{
			//GunAnimation->SetFlipbook(Flipbooks.Idle);
		}
	}
	
	
}

void AShootGun::PullTriger()
{

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	//if (OwnerController == nullptr) return;
	if (bIsShootingMode)
	{
		return;
	}
	

	FVector End = LocationPoint + RotationPoint.Vector() * MaxRange;

	if (MagazineAmmo <= 0)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Ammo left %i"), MagazineAmmo);
	FHitResult Hit;
	MagazineAmmo--;
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
	//GunAnimation->SetFlipbook(Flipbooks.Shot);

	GetWorldTimerManager().SetTimer(FireRateTimerHandle,
		[this]()
		{
			//GunAnimation->SetFlipbook(Flipbooks.Idle);
			bIsShoot = false;
			
		},
		0.25f, false);
		bIsShootingMode = true;
	GetWorldTimerManager().SetTimer(ShootingModeTimer,
		[this]()
		{

			bIsShootingMode = false;
		},
		0.75f, false);


	
}

void AShootGun::SetArrowLocation(FVector ShootPointLocation, FRotator ShotPointRotation)
{
	LocationPoint = ShootPointLocation;
	RotationPoint = ShotPointRotation;
}

void AShootGun::Reload()
{
	if (MaxAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("out of ammo!"));
		return;
	}
	int SubstractAmaut = 0;
	SubstractAmaut = 30 - MagazineAmmo;

	//UE_LOG(LogTemp, Warning, TEXT("r�nica: %i "),SubstractAmaut);
	
	if (SubstractAmaut > MaxAmmo)
	{
		SubstractAmaut = MaxAmmo;		
	}	
	MaxAmmo = MaxAmmo - SubstractAmaut;

	MagazineAmmo += SubstractAmaut;

	//UE_LOG(LogTemp, Warning, TEXT("masz tyle w magazynku %i i tyle ogolnie %i"),MagazineAmmo, MaxAmmo);

}