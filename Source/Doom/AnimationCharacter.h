// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "AnimationCharacter.generated.h"

class AShootGun;
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

	void Shoot();

	
	

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

private:
	// Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 200.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotateSpeed = 200.f;

	void Move(float Value);
	void Turn(float Value);

	void SideMove(float Value);
	void LookRight(float Value);


	void StepCamera();


	float RunningTime;

	bool bAnimation;


	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShootGun> GunClass;

	UPROPERTY()
	AShootGun* ShootGun;

};
