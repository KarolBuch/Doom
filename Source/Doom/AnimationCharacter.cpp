// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationCharacter.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ShootGun.h"

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

	ShootGun = GetWorld()->SpawnActor<AShootGun>(GunClass);
	
	ShootGun->AttachToComponent(GetSprite(), FAttachmentTransformRules::KeepRelativeTransform);
	ShootGun->SetOwner(this);

}

void AAnimationCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("you died"));
	}
}

void AAnimationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AAnimationCharacter::Move);
	PlayerInputComponent->BindAxis(TEXT("Side"), this, &AAnimationCharacter::SideMove);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AAnimationCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AAnimationCharacter::Shoot);
	
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

	ShootGun->SetArrowLocation(ArrowComp->GetComponentLocation(), ArrowComp->GetComponentRotation());
	ShootGun->PullTriger();
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

	return DamageToApply;

}

