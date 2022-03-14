// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootEffect.generated.h"

UCLASS()
class DOOM_API AShootEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShootEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	class UPaperFlipbookComponent* ShootAnimation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
