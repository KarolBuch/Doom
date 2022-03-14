// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootImpact.h"
#include "PaperFlipbookComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"

// Sets default values
AShootImpact::AShootImpact()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	ImpactAnimation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ImpactAnimation"));

	RootComponent = ImpactAnimation;

	

}

// Called when the game starts or when spawned
void AShootImpact::BeginPlay()
{
	Super::BeginPlay();

	//ImpactAnimation->OnComponentHit.AddDynamic(this, &AShootImpact::OnHit);

	GetWorldTimerManager().SetTimer(DestroyTimerHandle,
		[this]()
		{
			
			Destroy();
		},
		0.4f, false);
	
}

// Called every frame
void AShootImpact::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AShootImpact::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* otherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	UE_LOG(LogTemp, Warning, TEXT("OnHit!!!"));
//}

