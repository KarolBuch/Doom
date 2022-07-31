// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationCharacter.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BaseGun.h"
#include "ShotGun.h"
#include "DoubleFist.h"
#include "TimerManager.h"
#include "DoomGameModeBase.h"

AAnimationCharacter::AAnimationCharacter()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(RootComponent);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComp->SetupAttachment(CameraComp);

	GetSprite()->SetupAttachment(CameraComp);
	bIsMoving = false;
	
}

void AAnimationCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	
	CurrentWeapon = GetWorld()->SpawnActor<AShotGun>(ShootGunClass);
	
	CurrentWeapon->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepRelativeTransform);
	CurrentWeapon->SetOwner(this);
	WeaponIndex = 1;

}

void AAnimationCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead())
	{
		//UE_LOG(LogTemp, Warning, TEXT("you died"))
			return;
	}
	GetAmmo();


	CurrentWeapon->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepRelativeTransform);
}

void AAnimationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AAnimationCharacter::Move);
	PlayerInputComponent->BindAxis(TEXT("Side"), this, &AAnimationCharacter::SideMove);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AAnimationCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AAnimationCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &AAnimationCharacter::Reloading);
	PlayerInputComponent->BindAction(TEXT("PullOutFists"), EInputEvent::IE_Pressed, this, &AAnimationCharacter::SetFists);
	PlayerInputComponent->BindAction(TEXT("PullOutShootGun"), EInputEvent::IE_Pressed, this, &AAnimationCharacter::SetShootgun);
	
}
void AAnimationCharacter::Move(float Value)
{
	
	AddMovementInput(GetActorForwardVector() * Value * Speed * GetWorld()->GetDeltaSeconds());
	if (Value != 0)
	{
		StepCamera();
		bIsMoving = true;
	}
	else
	{
		bIsMoving = false;
	}	
}
void AAnimationCharacter::SideMove(float Value)
{
	AddMovementInput(GetActorRightVector() * Value * Speed * GetWorld()->GetDeltaSeconds());


	if (Value != 0 && bIsMoving == false)
	{
		StepCamera();
	}


}
void AAnimationCharacter::LookRight(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = Value * RotateSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation);

}

void  AAnimationCharacter::Shoot()
{
	if(!CurrentWeapon)
	{
		return;
	}
	
	CurrentWeapon->SetArrowLocation(ArrowComp->GetComponentLocation(), ArrowComp->GetComponentRotation());
	CurrentWeapon->PullTriger();
	
	
}

void AAnimationCharacter::StepCamera()
{
	FVector StartLocation = CameraComp->GetComponentLocation();
	
	float z = MaxMovementDisance.Z * FMath::Sin(Frequency.Z * RunningTime) + StartLocation.Z;
	RunningTime += UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector NewLocation = FVector(StartLocation.X, StartLocation.Y, z);


	CameraComp->SetWorldLocation(NewLocation);
}

float AAnimationCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);

	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

	if(IsDead())
	{
		ADoomGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ADoomGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		CurrentWeapon->Destroy();
		DetachFromControllerPendingDestroy();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	

	}

	return DamageToApply;

}

void AAnimationCharacter::Reloading()
{

	if (CurrentWeapon)
	{
	CurrentWeapon->Reload();
	}
	
	CurrentWeapon->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepRelativeTransform);
	CurrentWeapon->SetOwner(this);
	
}

void AAnimationCharacter::ChangeWeapon()
{

	if (WeaponIndex == 0)
	{
	
	CurrentWeapon->HideWeapon();
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AAnimationCharacter::SetDoubleFistToCharacter, 0.6, false);
	UE_LOG(LogTemp, Warning, TEXT("ebe"));
	}
	if (WeaponIndex == 1)
	{
		CurrentWeapon->HideWeapon();
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AAnimationCharacter::SetShootGunToCharacter, 0.6f, false);

	}
}
void  AAnimationCharacter::SetDoubleFistToCharacter()
{

	CurrentWeapon = GetWorld()->SpawnActor<ADoubleFist>(DoubleFistClass);
	CurrentWeapon->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepRelativeTransform);
	CurrentWeapon->SetOwner(this);
}

void AAnimationCharacter::SetShootGunToCharacter()
{

	CurrentWeapon = GetWorld()->SpawnActor<AShotGun>(ShootGunClass);
	CurrentWeapon->SetAmmo(ShootGunAmmo, ShootGunMagazine);
	CurrentWeapon->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepRelativeTransform);
	CurrentWeapon->SetOwner(this);
	

	
}
void AAnimationCharacter::SetFists()
{
	
	if (WeaponIndex != 0)
	{
	RememberAmmo();
	WeaponIndex = 0;

	ChangeWeapon();
	}
	

}

void AAnimationCharacter::SetShootgun()
{
	
	if (WeaponIndex != 1)
	{
	RememberAmmo();
	WeaponIndex = 1;
	ChangeWeapon();
	
	}

}

void AAnimationCharacter::RememberAmmo()
{
	if (WeaponIndex == 1)
	{
		ShootGunAmmo = CurrentWeapon->GetMaxAmmo();
		ShootGunMagazine = CurrentWeapon->GetMagazineAmmo();
	}
}
bool AAnimationCharacter::IsDead() const
{
	return Health <= 0;
}
float AAnimationCharacter::GetHealth() const
{
	return Health;
}
float AAnimationCharacter::GetAmmo() const
{
		return CurrentWeapon->GetMagazineAmmo();
}
float AAnimationCharacter::GetMaxAmmo() const
{
		return CurrentWeapon->GetMaxAmmo();
	
}
bool AAnimationCharacter::bIsFists() const
{
	if (WeaponIndex == 0)
	{
		return true;
	}
	return false;
 }

void AAnimationCharacter::GainHealth(float HealthValue)
{
	if (Health > 80 && Health != 100)
	{
		HealthValue = MaxHealth - Health;
	}
	UE_LOG(LogTemp, Warning, TEXT("tyle hp dodajesz %f"), HealthValue);
	Health = Health + HealthValue;

}