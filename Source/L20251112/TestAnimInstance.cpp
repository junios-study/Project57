// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAnimInstance.h"
#include "Gameframework/Character.h"
#include "Gameframework/CharacterMovementComponent.h"

void UTestAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
	if (Character)
	{
		GroundSpeed = Character->GetCharacterMovement()->Velocity.Size2D();
	}
}
