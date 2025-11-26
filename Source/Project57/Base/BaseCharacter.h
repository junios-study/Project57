// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UInputAction;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Unarmed = 0 UMETA(DisplayName = "Unarmed"),
	Pistol = 10 UMETA(DisplayName = "Pistol"),
	Rifle = 20 UMETA(DisplayName = "Rifle"),
	Launcher = 30 UMETA(DisplayName = "Launcher")
};


UCLASS()
class PROJECT57_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UChildActorComponent> Weapon;



public:
	FORCEINLINE class USpringArmComponent* GetSpringArm() const 
	{
		return SpringArm;
	}

	FORCEINLINE class UCameraComponent* GetCameraComponent() const
	{
		return Camera;
	}


	UFUNCTION(BlueprintCallable)
	void Move(float Forward, float Right);

	UFUNCTION(BlueprintCallable)
	void Look(float Pitch, float Yaw);

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void DoFire();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Character)
	uint8 bSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bLeftLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bRightLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bAiming : 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	EWeaponState WeaponState = EWeaponState::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Fire;
	


	UFUNCTION(BlueprintCallable)
	void HitReaction();

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float CurrentHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsFire : 1 = false;



	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	UFUNCTION(BlueprintCallable)
	void DoDeadEnd();

	UFUNCTION(BlueprintCallable)
	void DoDead();

	UFUNCTION(BlueprintCallable)
	void DoHitReact();


	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
