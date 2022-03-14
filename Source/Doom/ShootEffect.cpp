// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootEffect.h"
#include "PaperFlipbookComponent.h"

// Sets default values
AShootEffect::AShootEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ShootAnimation = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT(" Animaion Comp"));
	ShootAnimation->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AShootEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShootEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

