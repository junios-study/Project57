// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "../Weapon/WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Weapon/BaseDamageType.h"
#include "Engine/DamageEvents.h"
#include "PickupItemBase.h"
#include "Components/DecalComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	SetGenericTeamId(1);

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ABaseCharacter::ProcessBeginOverlap);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Reload, ETriggerEvent::Completed, this,
			&ABaseCharacter::Reload);

		UIC->BindAction(IA_Fire, ETriggerEvent::Started, this,
			&ABaseCharacter::StartFire);
		UIC->BindAction(IA_Fire, ETriggerEvent::Completed, this,
			&ABaseCharacter::StopFire);

		UIC->BindAction(IA_IronSight, ETriggerEvent::Started, this,
			&ABaseCharacter::StartIronSight);
		UIC->BindAction(IA_IronSight, ETriggerEvent::Completed, this,
			&ABaseCharacter::StopIronSight);
	}

}

void ABaseCharacter::Move(float Forward, float Right)
{
	const FRotator CameraRotation =  GetController()->GetControlRotation();
	const FRotator YawRotation = FRotator(0, CameraRotation.Yaw, 0);
	const FRotator YawRollRotation = FRotator(0, CameraRotation.Yaw, CameraRotation.Roll);


	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(YawRotation);
	AddMovementInput(ForwardVector, Forward);

	const FVector RightVector = UKismetMathLibrary::GetRightVector(YawRollRotation);
	AddMovementInput(RightVector, Right);


//	AddMovementInput(FVector(Forward, Right, 0));
}

void ABaseCharacter::Look(float Pitch, float Yaw)
{
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
}

void ABaseCharacter::Reload()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		PlayAnimMontage(ChildWeapon->ReloadMontage);
	}
}

void ABaseCharacter::DoFire()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->Fire();
	}
}

void ABaseCharacter::StartFire()
{
	bIsFire = true;
	DoFire();
}

void ABaseCharacter::StopFire()
{
	bIsFire = false;
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->StopFire();
	}
}

void ABaseCharacter::HitReaction()
{
	FString SectionName = FString::Printf(TEXT("%d"), FMath::RandRange(1, 8));

	PlayAnimMontage(HitMontage, 1.0, FName(*SectionName) );
}

void ABaseCharacter::ReloadWeapon()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->Reload();
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return DamageAmount;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
		}

		SpawnHitEffect(Event->HitInfo);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* Event = (FRadialDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;

			UE_LOG(LogTemp, Warning, TEXT("Radial Damage %f %s"), DamageAmount, *Event->DamageTypeClass->GetName());
		}
	}
	else //(DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		CurrentHP -= DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage %f"), DamageAmount);
	}

	DoHitReact();



	if (CurrentHP <= 0)
	{
		//죽는다. 애님 몽타주 재생
		//네트워크 할려면 다 RPC로 작업해 됨
		DoDead();
	}

	return DamageAmount;
}

void ABaseCharacter::DoDeadEnd()
{
	GetController()->SetActorEnableCollision(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ABaseCharacter::DoDead()
{
	FName SectionName = FName(FString::Printf(TEXT("%d"), FMath::RandRange(1, 6)));
	PlayAnimMontage(DeathMontage, 1.0f, SectionName);
}

void ABaseCharacter::DoHitReact()
{
	FName SectionName = FName(FString::Printf(TEXT("%d"), FMath::RandRange(1, 8)));
	PlayAnimMontage(HitMontage, 1.0f, SectionName);
}

void ABaseCharacter::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	APickupItemBase* PickedUpItem = Cast<APickupItemBase>(OtherActor);

	if (PickedUpItem)
	{
		switch (PickedUpItem->ItemType)
		{
		case EItemType::Use: //Game Ability System
			UseItem(PickedUpItem);
			break;
		case EItemType::Eat:
			EatItem(PickedUpItem);
			break;
		case EItemType::Equip:
			EquipItem(PickedUpItem);
			break;
		}

		if (!PickedUpItem->bIsInfinity)
		{
			PickedUpItem->Destroy();
		}
	
	}
}

void ABaseCharacter::EatItem(APickupItemBase* PickedUpItem)
{
}

void ABaseCharacter::UseItem(APickupItemBase* PickedUpItem)
{
}

void ABaseCharacter::EquipItem(APickupItemBase* PickedUpItem)
{
	Weapon->SetChildActorClass(PickedUpItem->ItemTemplate);
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());

	if (ChildWeapon)
	{
		if (ChildWeapon->Name.Compare(TEXT("Pistol")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Pistol;
			ChildWeapon->SetOwner(this);
		}
		else if(ChildWeapon->Name.Compare(TEXT("Rifle")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			ChildWeapon->SetOwner(this);
		}
		else if (ChildWeapon->Name.Compare(TEXT("GrenadeLauncer")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			//WeaponState = EWeaponState::GrenadeLauncer;
			ChildWeapon->SetOwner(this);
		}
	}
}

void ABaseCharacter::StartIronSight()
{
	bIsIronSight = true;
}

void ABaseCharacter::StopIronSight()
{
	bIsIronSight = false;
}

//----------------------------------------------------------------------//
// IGenericTeamAgentInterface
//----------------------------------------------------------------------//
void ABaseCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamID = InTeamID;
}

FGenericTeamId ABaseCharacter::GetGenericTeamId() const
{
	return TeamID;
}




void ABaseCharacter::SpawnHitEffect(FHitResult Hit)
{
	if (BloodEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("BloodEffect"));
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}
}
