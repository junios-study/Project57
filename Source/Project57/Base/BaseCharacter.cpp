// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "../Weapon/WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"



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

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//무기 집으면 잡게 이동
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
		WeaponState = EWeaponState::Pistol;
	}

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
		UIC->BindAction(IA_Fire, ETriggerEvent::Triggered, this,
			&ABaseCharacter::DoFire);
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
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		int32 SizeX = 0;
		int32 SizeY = 0;
		int32 CenterX = 0;
		int32 CenterY = 0;
		FVector WorldDirection;
		FVector WorldLocation;
		FVector CameraLocation;
		FRotator CameraRotation;

		PC->GetViewportSize(SizeX, SizeY);
		CenterX = SizeX / 2;
		CenterY = SizeY / 2;
		
		PC->DeprojectScreenPositionToWorld((float)CenterX, (float)CenterY,
			WorldLocation, WorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector Start = CameraLocation;
		FVector End = CameraLocation + WorldDirection * 100000.0f;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> IngnoreActors;
		FHitResult HitResult;

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			ObjectTypes,
			true,
			IngnoreActors,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true
		);

		if (bResult)
		{
			//HitResult.GetActor();
		}



	}
	//AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	//if (ChildWeapon)
	//{
	//	ChildWeapon->Fire();
	//}
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
