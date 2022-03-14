// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootGun.generated.h"

UENUM(BlueprintType, Category = "Animation")
enum class EAnimationDestructor : uint8
{
	Idle,
	Walking,
	Shot,
	HideWeapon,
};

USTRUCT(BlueprintType, Category = "Animation")
struct FAnimationFlipbooks
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* Idle{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* Walking{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* Shot{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* HideWeapon{ nullptr };

};
UCLASS()
class DOOM_API AShootGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShootGun();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationDestructor")
	EAnimationDestructor CurrentAnimationWeapon;

	UPROPERTY(EditAnywhere, Category = "AnimationDestructor")
	FAnimationFlipbooks Flipbooks;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* GunAnimation;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeAnimation();

	void PullTriger();

	void SetArrowLocation(FVector ShootPointLocation, FRotator ShotPointRotation);

	bool bIsShoot;

	bool bIsShootingMode;

	

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	float ShootAnim = 0.165f;

	FTimerHandle FireRateTimerHandle;

	FTimerHandle ShootingModeTimer;
	
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	FVector LocationPoint;
	FRotator RotationPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AShootImpact> ImpactClass;

	UPROPERTY()
	AShootImpact* ImpactEffect;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

};
