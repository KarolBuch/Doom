// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "Animation")
enum class EEnemyDestructor : uint8
{
	Idle,
	Walking,
	Shot,
	HideWeapon,
};
USTRUCT(BlueprintType, Category = "Animation")
struct FEnemyFlipbooks
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
class DOOM_API ABaseEnemy : public APaperCharacter
{
	GENERATED_BODY()

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float RunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trignometric Values")
	FVector Frequency;

	UPROPERTY(VisibleAnywhere, Category = "Trignometric Values")
	FVector StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trignometric Values")
	FVector MaxMovementDisance;

	AActor* Parent = GetOwner();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateActor(FVector LookAtTarget);

	void ChangeFlipbook(FRotator PoseToTarget);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationDestructor")
		EEnemyDestructor CurrentAnimationWeapon;

	UPROPERTY(EditAnywhere, Category = "AnimationDestructor")
		FEnemyFlipbooks Flipbooks;

private:

	class AAnimationCharacter* PlayerChar;

	float RotateRange = 3000.f;
	
};
