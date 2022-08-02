// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

UENUM(BlueprintType, Category = "Animation")
enum class EAnimationCharacter : uint8
{
	Idle,
	Walking,
	Shot,
	HideWeapon,
	PullOutWeapon,
};

USTRUCT(BlueprintType, Category = "Animation")
struct FAnimationCombatFlipbooks
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* PullOutWeapon{ nullptr };

};

UCLASS()
class DOOM_API ABaseGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGun();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationDestructor")
		EAnimationCharacter CurrentAnimationWeapon;

	UPROPERTY(EditAnywhere, Category = "AnimationDestructor")
		FAnimationCombatFlipbooks Flipbooks;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* GunAnimation;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeAnimation();

	void PullTriger();

	void Reload();

	void SetArrowLocation(FVector ShootPointLocation, FRotator ShotPointRotation);

	void HideWeapon();

	void PullOutWeapon();

	bool bIsShoot;

	bool bIsShootingMode;

	bool bIsPullingOut;

	int GetMaxAmmo();

	int GetMagazineAmmo();

	void SetAmmo(int MaxAmmoValue, int MagazineAmmoValue);

	
private:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	float ShootAnim = 0.165f;

	FTimerHandle FireRateTimerHandle;

	FTimerHandle ShootingModeTimer;

	FTimerHandle HidingWeaponTimer;
	FTimerHandle PullingOutWeaponTimer;

	UPROPERTY(EditAnywhere)
		float MaxRange = 1000;
	UPROPERTY(EditAnywhere)
		int MaxAmmo = 120;
	UPROPERTY(EditAnywhere)
		int MagazineAmmo;
	UPROPERTY(EditAnywhere)
		float AttackDelay = 1.f;
	UPROPERTY(EditAnywhere)
		bool bIsInfiniteAmmo;


	FVector LocationPoint;
	FRotator RotationPoint;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AShootImpact> ImpactClass;

	UPROPERTY()
		AShootImpact* ImpactEffect;

	UPROPERTY(EditAnywhere)
		USoundBase* ShotGunSound;

	UPROPERTY(EditAnywhere)
		USoundBase* ShotGunReloadSound;

	UPROPERTY(EditAnywhere)
		USoundBase* TakePunchSound;

	UPROPERTY(EditAnywhere)
		float Damage = 10;
};
