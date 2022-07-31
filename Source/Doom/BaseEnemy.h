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
		class UPaperFlipbook* dying{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* HideWeapon{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* Attack{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* LeftIdle{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* rightIdle{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbook* BackIdle{ nullptr };



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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UPROPERTY(EditAnywhere)
	class APatrolPath* PatrolPath;

	bool IsDead() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateActor(FVector LookAtTarget);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationDestructor")
		EEnemyDestructor CurrentAnimationWeapon;

	UPROPERTY(EditAnywhere, Category = "AnimationDestructor")
		FEnemyFlipbooks Flipbooks;



	

private:

	//Combat
	float RotateRange = 3000.f;
	float AttackRange = 280.f;
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
	UPROPERTY(EditDefaultsOnly)
	float Health;
	void DeadAnimation();
	void CheckAttackCondition();
	bool InAttackRange();
	void Attack();
	
	UPROPERTY(EditAnywhere)
	float AttackSpeed;
	class AAnimationCharacter* PlayerChar;
	
	bool bIsDead;

	FTimerHandle AttackTimer;

	bool bIsAttacking;

	FTimerHandle DestroyTimerHandle;


	void ChangeAnimation();

};
