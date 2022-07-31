// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthGain.generated.h"

class AAnimationCharacter;

UCLASS()
class DOOM_API AHealthGain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthGain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UPaperFlipbookComponent* Heart;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	float HealthValue;

	class AAnimationCharacter* PlayerChar;
	void RotateActor(FVector LookAtTarget);

	
};
