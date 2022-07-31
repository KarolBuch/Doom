// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGun.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "ShootImpact.h"


// Sets default values
ABaseGun::ABaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);

	GunAnimation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AnimationComp"));
	GunAnimation->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay()
{
	Super::BeginPlay();

	bIsShoot = false;
	bIsPullingOut = true;
	PullOutWeapon();
	
	Reload();
	
}

// Called every frame
void ABaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeAnimation();
}

void ABaseGun::ChangeAnimation()
{

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	FVector OwnerVelocity = OwnerPawn->GetVelocity();
	if (bIsShoot == false && !bIsPullingOut)
	{

		if (OwnerVelocity.X && OwnerVelocity.Y != 0 && !bIsShootingMode)
		{
			GunAnimation->SetFlipbook(Flipbooks.Walking);
		}
		else if (OwnerVelocity.X == 0  || bIsShootingMode)
		{
			GunAnimation->SetFlipbook(Flipbooks.Idle);
		}
	}


}

void ABaseGun::PullTriger()
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

	if (MagazineAmmo <= 0 && !bIsInfiniteAmmo)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Ammo left %i"), MagazineAmmo);
	FHitResult Hit;
	if (!bIsInfiniteAmmo)
	{
		MagazineAmmo--;
	}
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, LocationPoint, End, ECollisionChannel::ECC_GameTraceChannel1);
	if (bSuccess)
	{
		FVector ShotDirection = -RotationPoint.Vector();
		//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Cyan, true);
		GetWorld()->SpawnActor<AShootImpact>(ImpactClass, Hit.Location, RotationPoint);
		FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
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

		},
		0.25f, false);
	bIsShootingMode = true;
	GetWorldTimerManager().SetTimer(ShootingModeTimer,
		[this]()
		{

			bIsShootingMode = false;
		},
		AttackDelay, false);


	UE_LOG(LogTemp, Warning, TEXT("masz tyle w magazynku %i i tyle ogolnie %i"), MagazineAmmo, MaxAmmo);
}

void ABaseGun::SetArrowLocation(FVector ShootPointLocation, FRotator ShotPointRotation)
{
	LocationPoint = ShootPointLocation;
	RotationPoint = ShotPointRotation;
}

void ABaseGun::Reload()
{
	if (MagazineAmmo == 30)
	{
		UE_LOG(LogTemp, Warning, TEXT("out of ammo!"));
		return;
		
	}
	if (MaxAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("out of ammo!"));
		return;
	}
	int SubstractAmaunt = 0;
	SubstractAmaunt = 2 - MagazineAmmo;

	//UE_LOG(LogTemp, Warning, TEXT("ró¿nica: %i "),SubstractAmaut);

	if (SubstractAmaunt > MaxAmmo)
	{
		SubstractAmaunt = MaxAmmo;
	}
	MaxAmmo = MaxAmmo - SubstractAmaunt;

	MagazineAmmo += SubstractAmaunt;


	//UE_LOG(LogTemp, Warning, TEXT("masz tyle w magazynku %i i tyle ogolnie %i"),MagazineAmmo, MaxAmmo);

}

void ABaseGun::HideWeapon()
{
	
	GunAnimation->SetFlipbook(Flipbooks.HideWeapon);
	bIsShoot = true;

	GetWorldTimerManager().SetTimer(HidingWeaponTimer,
		[this]()
		{
			Destroy();
		},
		0.6f,false);



}

void ABaseGun::PullOutWeapon()
{
	GunAnimation->SetFlipbook(Flipbooks.PullOutWeapon);
	GetWorldTimerManager().SetTimer(HidingWeaponTimer,
		[this]()
		{
			bIsPullingOut = false;

		},
		0.826f, false);
}

int ABaseGun::GetMaxAmmo()
{
	return MaxAmmo;
}

int ABaseGun::GetMagazineAmmo()
{
	return MagazineAmmo;
}

void ABaseGun::SetAmmo(int MaxAmmoValue, int MagazineAmmoValue)
{
	MaxAmmo = MaxAmmoValue;
	MagazineAmmo = MagazineAmmoValue;
}