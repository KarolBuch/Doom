// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "AnimationCharacter.generated.h"

class ABaseGun;
/**
 * 
 */

UCLASS()
class DOOM_API AAnimationCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:

	AAnimationCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowComp;

	virtual void BeginPlay() override;

	uint8 bIsMoving : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trignometric Values")
	FVector Frequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trignometric Values")
	FVector MaxMovementDisance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int WeaponIndex;



private:
	// Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotateSpeed = 200.f;

	void SideMove(float Value);
	void LookRight(float Value);
	void Move(float Value);
	void Turn(float Value);
	void StepCamera();
	float RunningTime;

	// Combat
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(EditDefaultsOnly)
	float Health;

	int ShootGunAmmo;
	int ShootGunMagazine;

	void Shoot();
	void ChangeWeapon();
	void SetFists();
	void SetShootgun();
	void SetDoubleFistToCharacter();
	void Reloading();
	void SetShootGunToCharacter();
	void RememberAmmo();

	

	bool bAnimation;

	FTimerHandle DestroyTimer;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseGun> ShootGunClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseGun> DoubleFistClass;


	UPROPERTY()
	ABaseGun* CurrentWeapon;

};