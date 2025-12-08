// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseDamageType.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "ProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Project57.h"
#include "../Network/NetworkUtil.h"
#include "../Base/BaseCharacter.h"
#include "Camera/CameraComponent.h"



// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		NET_LOG(FString::Printf(TEXT("Weapon Owner %s"), *GetOwner()->GetName()));
	}
	else
	{
		NET_LOG(FString::Printf(TEXT("No Owner")));
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Reload()
{
	CurrentBulletCount = MaxBulletCount;
	UE_LOG(LogTemp, Warning, TEXT("Reload %d"), CurrentBulletCount);
}

void AWeaponBase::Fire()
{
	float CurrentTimeofShoot = GetWorld()->TimeSeconds - TimeofLastShoot;

	if (CurrentTimeofShoot < RefireRate)
	{
		return;
	}

	if (bFullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AWeaponBase::Fire, RefireRate, false);
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character)
	{
		return;
	}

	//	//Calculate
	FVector SpawnLocation;
	FVector TargetLocation;
	//FVector BulletDirection;
	FRotator AimRotation;
	FHitResult HitResult;

	//화면 가운데 기준이 아니라 총 Muzzle 앞 방향으로 발사.
	//SpawnLocation = Mesh->GetSocketLocation(TEXT("Muzzle"));
	//FVector WeaponForward = Mesh->GetSocketRotation(TEXT("Muzzle")).Vector().GetSafeNormal();
	//TargetLocation = SpawnLocation + (WeaponForward * 100000.0f);
	//AimRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation + (UKismetMathLibrary::RandomUnitVector() * 0.3f));

	bool bResult = CalculateShootData(SpawnLocation, TargetLocation, AimRotation);
	if (!bResult)
	{
		return;
	}

	FireProjectile(FTransform(AimRotation, SpawnLocation, FVector::OneVector),
		HitResult);

	//호출은 서버에서 모든 클라이언에서 실행
	S2A_SpawnMuzzleFlash(SpawnLocation, AimRotation);

	//Recoil
	Character->AddControllerPitchInput(-0.05f);


	CurrentBulletCount--;
	UE_LOG(LogTemp, Warning, TEXT("Fire %d"), CurrentBulletCount);

	//호출은 서버에서 모든 클라이언에서 실행
	S2A_SpawnFireSound(SpawnLocation);

	TimeofLastShoot = GetWorld()->TimeSeconds;

}

void AWeaponBase::FireProjectile(FTransform SpawnTransform, FHitResult InHitResult)
{
	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileTemplate, SpawnTransform);
	Projectile->HitResult = InHitResult;
	Projectile->SetOwner(this);
}

bool AWeaponBase::CalculateShootData(FVector& OutSpawnLocation, FVector& OutTargetLocation, FRotator& OutAimRotation)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());

	if (!Character)
	{
		return false;
	}

	FHitResult OutHit;

	const FVector Start = Character->GetCameraComponent()->GetComponentLocation();
	const FVector End = Start + (Character->GetCameraComponent()->GetForwardVector() * 10000.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(Character);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	//Calculate
	OutSpawnLocation = Mesh->GetSocketLocation(TEXT("Muzzle"));
	OutTargetLocation = OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;

	OutAimRotation = UKismetMathLibrary::FindLookAtRotation(OutSpawnLocation, OutTargetLocation + (UKismetMathLibrary::RandomUnitVector() * 0.3f));
	
	return true;
}

//Execute Client 실행
void AWeaponBase::S2A_SpawnMuzzleFlash_Implementation(const FVector& SpawnLocation, const FRotator& AimRotation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash,
		SpawnLocation,
		AimRotation
	);
}

void AWeaponBase::S2A_SpawnFireSound_Implementation(const FVector& SpawnLocation)
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, SpawnLocation);
}


void AWeaponBase::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

FVector AWeaponBase::GetWeaponTargetLocation()
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner());

	if (!Character)
	{
		return FVector::Zero();
	}

	FHitResult OutHit;

	const FVector Start = Character->GetCameraComponent()->GetComponentLocation();
	const FVector End = Start + (Character->GetCameraComponent()->GetForwardVector() * 10000.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	// return either the impact point or the trace end
	return OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
}


