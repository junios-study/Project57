// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_AIC.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../Base/BaseCharacter.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Zombie.h"

AZombie_AIC::AZombie_AIC()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	Sight->SightRadius = 800.0f;
	Sight->LoseSightRadius = 900.0f;
	Sight->PeripheralVisionAngleDegrees = 45.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());



}

void AZombie_AIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//ensure(RunBTAsset); //Log
	//check(RunBTAsset); //우선 종료 후 로그 확인
	if (RunBTAsset)
	{
		RunBehaviorTree(RunBTAsset);
	}

//	Perception->OnPerceptionUpdated.AddDynamic(this, &AZombie_AIC::ProcessPerception);
//	Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AZombie_AIC::ProcessActorPerceptionInfo);
	Perception->OnTargetPerceptionForgotten.AddDynamic(this, &AZombie_AIC::ProcessPerceptionForget);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &AZombie_AIC::ProcessActorPerception);
	SetGenericTeamId(3);
}

void AZombie_AIC::OnUnPossess()
{
	Super::OnUnPossess();
}

void AZombie_AIC::ProcessPerception(const TArray<AActor*>& UpdatedActors)
{
	for (auto Actor : UpdatedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProcessPerception %s"), *Actor->GetName());
	}
}

void AZombie_AIC::ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("ProcessActorPerception %s %s"), *Actor->GetName() ,
	//	*Stimulus.Type.Name.ToString());

	//시야
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Type Sight"));

		ABaseCharacter* Player = Cast<ABaseCharacter>(Actor);
		AZombie* Zombie = Cast<AZombie>(GetPawn());
		if (Player && Zombie)
		{
			Blackboard->SetValueAsObject(TEXT("Target"), Player);
			Blackboard->SetValueAsEnum(TEXT("CurrentState"), (uint8)(EZombieState::Chase));
			Zombie->SetState(EZombieState::Chase);
			Zombie->ChangeSpeed(400.0f);
		}
	}

}

void AZombie_AIC::ProcessPerceptionForget(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessPerceptionForget %s"), *Actor->GetName());

	ABaseCharacter* Player = Cast<ABaseCharacter>(Actor);
	AZombie* Zombie = Cast<AZombie>(GetPawn());
	if (Player && Zombie)
	{
		Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
		Blackboard->SetValueAsEnum(TEXT("CurrentState"), (uint8)(EZombieState::Normal));
		Zombie->SetState(EZombieState::Normal);
		Zombie->ChangeSpeed(80.0f);
	}

}

void AZombie_AIC::ProcessActorPerceptionInfo(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessPerceptionForget %s"), *(UpdateInfo.Target->GetName()));
}
