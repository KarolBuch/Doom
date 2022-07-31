// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthGain.h"
#include "Components/SphereComponent.h"
#include "AnimationCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AHealthGain::AHealthGain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapRadius"));
	Sphere->SetupAttachment(RootComponent);

	Heart = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Animation"));
	Heart->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealthGain::BeginPlay()
{
	Super::BeginPlay();

	PlayerChar = Cast<AAnimationCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	
}

// Called every frame
void AHealthGain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	

	if(PlayerChar)
	{
		
			RotateActor(PlayerChar->GetActorLocation());

			
			if (Sphere->IsOverlappingActor(PlayerChar))
			{
			if (PlayerChar->GetHealth() == 100)
			{return;}
			PlayerChar->GainHealth(HealthValue);
			Destroy();
			}
		
	}

}

void AHealthGain::RotateActor(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - RootComponent->GetComponentLocation();

	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);


	Heart->SetRelativeRotation(LookAtRotation);
}



