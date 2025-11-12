// Fill out your copyright notice in the Description page of Project Settings.


#include "TestAnimInstance.h"
#include "TestCharacter.h"
#include "Gameframework/CharacterMovementComponent.h"

void UTestAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ATestCharacter* Character = Cast<ATestCharacter>(TryGetPawnOwner());
	if (Character)
	{
		GroundSpeed = Character->GetCharacterMovement()->Velocity.Size2D();
		bSprint = Character->bSprint;
	}
}
